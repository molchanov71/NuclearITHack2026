package com.hexteam.app.discovery;

import com.hexteam.app.config.HexProperties;
import com.hexteam.app.metrics.DiagnosticsService;
import com.hexteam.app.security.NodeIdentity;
import com.hexteam.app.security.NodeIdentityService;
import jakarta.annotation.PreDestroy;
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
import java.net.SocketException;
import java.time.Clock;
import java.time.Instant;
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
    private final DiagnosticsService diagnosticsService;
    private final Clock clock;
    private final int serverPort;
    private final AtomicLong sequence = new AtomicLong();
    private final AtomicBoolean running = new AtomicBoolean(false);
    private final ExecutorService executor = Executors.newSingleThreadExecutor();

    private DatagramSocket listenerSocket;

    public DiscoveryService(HexProperties properties,
                            NodeIdentityService identityService,
                            DiscoveryCodec codec,
                            PeerRegistry peerRegistry,
                            DiagnosticsService diagnosticsService,
                            Clock clock,
                            @Value("${server.port}") int serverPort) {
        this.properties = properties;
        this.identityService = identityService;
        this.codec = codec;
        this.peerRegistry = peerRegistry;
        this.diagnosticsService = diagnosticsService;
        this.clock = clock;
        this.serverPort = serverPort;
        startListener();
    }

    @Scheduled(fixedDelayString = "${hex.discovery.interval-ms:3000}")
    public void sendAnnounce() {
        DiscoveryAnnounce announce = buildAnnounce();
        byte[] payload = codec.write(announce);
        try (DatagramSocket socket = new DatagramSocket()) {
            socket.setBroadcast(true);
            DatagramPacket packet = new DatagramPacket(
                    payload,
                    payload.length,
                    InetAddress.getByName("255.255.255.255"),
                    properties.getDiscovery().getPort()
            );
            socket.send(packet);
            diagnosticsService.record("discovery", "Отправлен announce seq=" + announce.seq());
        } catch (IOException exception) {
            log.warn("Не удалось отправить discovery announce", exception);
            diagnosticsService.record("discovery", "Ошибка отправки announce: " + exception.getMessage());
        }
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
            listenerSocket = new DatagramSocket(properties.getDiscovery().getPort());
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

    private DiscoveryAnnounce buildAnnounce() {
        NodeIdentity identity = identityService.currentIdentity();
        return new DiscoveryAnnounce(
                identity.nodeId(),
                identity.displayName(),
                "1.0",
                "http://localhost:" + serverPort + properties.getSignal().getBasePath(),
                identity.fingerprint(),
                Set.of("chat", "audio", "files", "metrics"),
                Instant.now(clock),
                sequence.incrementAndGet()
        );
    }
}
