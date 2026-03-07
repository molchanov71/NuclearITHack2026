package com.hexteam.app.discovery;

import com.hexteam.app.config.HexProperties;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.hexteam.app.metrics.DiagnosticsService;
import com.hexteam.app.security.NodeIdentity;
import com.hexteam.app.security.NodeIdentityService;
import jakarta.annotation.PreDestroy;
import jakarta.annotation.PostConstruct;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.autoconfigure.condition.ConditionalOnProperty;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.net.SocketException;
import java.time.Duration;
import java.time.Clock;
import java.time.Instant;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicLong;

@Service
@ConditionalOnProperty(prefix = "hex.discovery", name = "enabled", havingValue = "true", matchIfMissing = true)
public class DiscoveryService {
    /**
     * Отдельный модуль discovery:
     * шлёт broadcast announce и слушает объявления других узлов.
     */

    private static final Logger log = LoggerFactory.getLogger(DiscoveryService.class);

    private final HexProperties properties;
    private final NodeIdentityService identityService;
    private final DiscoveryCodec codec;
    private final PeerRegistry peerRegistry;
    private final NetworkAddressResolver networkAddressResolver;
    private final DiagnosticsService diagnosticsService;
    private final Clock clock;
    private final ObjectMapper objectMapper;
    private final int serverPort;
    private final AtomicLong sequence = new AtomicLong();
    private final AtomicBoolean running = new AtomicBoolean(false);
    private final ExecutorService executor = Executors.newSingleThreadExecutor();

    private DatagramSocket listenerSocket;

    public DiscoveryService(HexProperties properties,
                            NodeIdentityService identityService,
                            DiscoveryCodec codec,
                            PeerRegistry peerRegistry,
                            NetworkAddressResolver networkAddressResolver,
                            DiagnosticsService diagnosticsService,
                            Clock clock,
                            ObjectMapper objectMapper,
                            @Value("${server.port}") int serverPort) {
        this.properties = properties;
        this.identityService = identityService;
        this.codec = codec;
        this.peerRegistry = peerRegistry;
        this.networkAddressResolver = networkAddressResolver;
        this.diagnosticsService = diagnosticsService;
        this.clock = clock;
        this.objectMapper = objectMapper;
        this.serverPort = serverPort;
        startListener();
    }

    @PostConstruct
    public void logNetworkConfiguration() {
        InetAddress primaryAddress = networkAddressResolver.resolvePrimaryAddress();
        List<InetAddress> broadcasts = networkAddressResolver.broadcastAddresses();
        String broadcastSummary = broadcasts.stream()
                .map(InetAddress::getHostAddress)
                .distinct()
                .sorted()
                .reduce((left, right) -> left + ", " + right)
                .orElse("<none>");
        log.info("Discovery настроен: primaryAddress={}, discoveryPort={}, broadcasts={}",
                primaryAddress.getHostAddress(),
                properties.getDiscovery().getPort(),
                broadcastSummary);
        diagnosticsService.record("discovery", "Primary IP: " + primaryAddress.getHostAddress());
        diagnosticsService.record("discovery", "Broadcast targets: " + broadcastSummary);
    }

    @Scheduled(fixedDelayString = "${hex.discovery.interval-ms:3000}")
    public void sendAnnounce() {
        DiscoveryAnnounce announce = buildAnnounce();
        byte[] payload = codec.write(announce);
        List<DiscoveryTarget> targets = networkAddressResolver.discoveryTargets();
        if (targets.isEmpty()) {
            diagnosticsService.record("discovery", "Не найдено broadcast-адресов для отправки announce");
            return;
        }
        for (DiscoveryTarget target : targets) {
            try (DatagramSocket socket = new DatagramSocket(new InetSocketAddress(target.localAddress(), 0))) {
                socket.setBroadcast(true);
                DatagramPacket packet = new DatagramPacket(
                        payload,
                        payload.length,
                        target.broadcastAddress(),
                        properties.getDiscovery().getPort()
                );
                socket.send(packet);
                diagnosticsService.record(
                        "discovery",
                        "Отправлен announce seq=" + announce.seq()
                                + " с " + target.localAddress().getHostAddress()
                                + " на " + target.broadcastAddress().getHostAddress()
                );
            } catch (IOException exception) {
                log.warn("Не удалось отправить discovery announce через {}", target.localAddress().getHostAddress(), exception);
                diagnosticsService.record(
                        "discovery",
                        "Ошибка отправки announce через " + target.localAddress().getHostAddress() + ": " + exception.getMessage()
                );
            }
        }
    }

    @Scheduled(fixedDelayString = "${hex.discovery.interval-ms:3000}", initialDelay = 1500)
    public void probePeersOverHttp() {
        if (!properties.getDiscovery().isProbeEnabled()) {
            return;
        }
        HttpClient client = HttpClient.newBuilder()
                .connectTimeout(Duration.ofMillis(properties.getDiscovery().getProbeTimeoutMs()))
                .build();
        networkAddressResolver.probeAddresses()
                .parallelStream()
                .forEach(address -> probeSingleAddress(client, address));
    }

    @PreDestroy
    public void shutdown() {
        running.set(false);
        if (listenerSocket != null && !listenerSocket.isClosed()) {
            listenerSocket.close();
        }
        executor.shutdownNow();
    }

    private void startListener() {
        try {
            listenerSocket = new DatagramSocket(null);
            listenerSocket.setReuseAddress(true);
            listenerSocket.setBroadcast(true);
            listenerSocket.bind(new InetSocketAddress(properties.getDiscovery().getPort()));
            running.set(true);
            executor.submit(this::listenLoop);
        } catch (SocketException exception) {
            throw new IllegalStateException("Не удалось открыть discovery listener", exception);
        }
    }

    private void listenLoop() {
        while (running.get()) {
            try {
                byte[] buffer = new byte[4096];
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                listenerSocket.receive(packet);
                DiscoveryAnnounce announce = codec.read(packet.getData(), packet.getLength());
                if (announce.nodeId().equals(identityService.currentIdentity().nodeId())) {
                    continue;
                }
                peerRegistry.upsert(announce);
                diagnosticsService.record("discovery", "Получен announce от " + announce.displayName());
            } catch (IOException exception) {
                if (running.get()) {
                    log.warn("Ошибка discovery listener", exception);
                    diagnosticsService.record("discovery", "Ошибка listener: " + exception.getMessage());
                }
            }
        }
    }

    public DiscoveryAnnounce currentAnnounceSnapshot() {
        return buildAnnounce();
    }

    private DiscoveryAnnounce buildAnnounce() {
        NodeIdentity identity = identityService.currentIdentity();
        InetAddress localAddress = networkAddressResolver.resolvePrimaryAddress();
        return new DiscoveryAnnounce(
                identity.nodeId(),
                identity.displayName(),
                "1.0",
                "http://" + localAddress.getHostAddress() + ":" + serverPort + properties.getSignal().getBasePath(),
                identity.fingerprint(),
                Set.of("chat", "audio", "files", "metrics"),
                Instant.now(clock),
                sequence.incrementAndGet()
        );
    }

    private void probeSingleAddress(HttpClient client, InetAddress address) {
        String localIp = networkAddressResolver.resolvePrimaryAddress().getHostAddress();
        if (address.getHostAddress().equals(localIp)) {
            return;
        }
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create("http://" + address.getHostAddress() + ":" + serverPort + "/api/discovery/probe"))
                .timeout(Duration.ofMillis(properties.getDiscovery().getProbeTimeoutMs()))
                .GET()
                .build();
        try {
            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            if (response.statusCode() != 200) {
                return;
            }
            DiscoveryAnnounce announce = objectMapper.readValue(response.body(), DiscoveryAnnounce.class);
            if (!announce.nodeId().equals(identityService.currentIdentity().nodeId())) {
                peerRegistry.upsert(announce);
                diagnosticsService.record("discovery", "HTTP probe обнаружил peer " + announce.displayName() + " по " + address.getHostAddress());
            }
        } catch (Exception ignored) {
            // Для probe-сканирования отсутствие ответа — нормальный сценарий.
        }
    }
}
