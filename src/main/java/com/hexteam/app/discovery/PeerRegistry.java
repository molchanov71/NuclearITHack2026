package com.hexteam.app.discovery;

import com.hexteam.app.config.HexProperties;
import com.hexteam.app.metrics.DiagnosticsService;
import com.hexteam.app.security.TrustState;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import java.time.Clock;
import java.time.Instant;
import java.util.Collection;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

@Service
public class PeerRegistry {
    /**
     * In-memory реестр peer-ов.
     * Здесь хранится последняя известная информация о каждом узле и его состоянии.
     */

    private final Map<String, PeerDescriptor> peers = new ConcurrentHashMap<>();
    private final HexProperties properties;
    private final Clock clock;
    private final DiagnosticsService diagnosticsService;

    public PeerRegistry(HexProperties properties, Clock clock, DiagnosticsService diagnosticsService) {
        this.properties = properties;
        this.clock = clock;
        this.diagnosticsService = diagnosticsService;
    }

    /**
     * Добавляет peer-а или обновляет уже известную запись по очередному announce.
     */
    public PeerDescriptor upsert(DiscoveryAnnounce announce) {
        Instant now = Instant.now(clock);
        PeerDescriptor descriptor = new PeerDescriptor(
                announce.nodeId(),
                announce.displayName(),
                announce.protocolVersion(),
                announce.signalBaseUrl(),
                announce.publicKeyFingerprint(),
                announce.capabilities(),
                announce.seq(),
                now,
                announce.timestamp(),
                PeerState.ONLINE,
                TrustState.UNVERIFIED
        );
        peers.put(announce.nodeId(), descriptor);
        diagnosticsService.record("discovery", "Peer " + announce.displayName() + " (" + announce.nodeId() + ") ONLINE");
        return descriptor;
    }

    public Collection<PeerDescriptor> allPeers() {
        return peers.values().stream()
                .sorted((left, right) -> right.lastSeen().compareTo(left.lastSeen()))
                .toList();
    }

    public int countOnlinePeers() {
        return (int) peers.values().stream().filter(peer -> peer.state() == PeerState.ONLINE).count();
    }

    public PeerDescriptor findByNodeId(String nodeId) {
        return peers.get(nodeId);
    }

    /**
     * Периодически переводит peer-ов в STALE/OFFLINE по TTL.
     */
    @Scheduled(fixedDelay = 1000)
    public void updatePeerStates() {
        Instant now = Instant.now(clock);
        long staleThreshold = properties.getDiscovery().getTtlMs() / 2;
        for (Map.Entry<String, PeerDescriptor> entry : peers.entrySet()) {
            PeerDescriptor peer = entry.getValue();
            long ageMs = now.toEpochMilli() - peer.lastSeen().toEpochMilli();
            PeerState newState = ageMs >= properties.getDiscovery().getTtlMs()
                    ? PeerState.OFFLINE
                    : ageMs >= staleThreshold ? PeerState.STALE : PeerState.ONLINE;
            if (newState != peer.state()) {
                PeerDescriptor updated = new PeerDescriptor(
                        peer.nodeId(),
                        peer.displayName(),
                        peer.protocolVersion(),
                        peer.signalBaseUrl(),
                        peer.publicKeyFingerprint(),
                        peer.capabilities(),
                        peer.seq(),
                        peer.lastSeen(),
                        peer.announcedAt(),
                        newState,
                        peer.trustState()
                );
                peers.put(entry.getKey(), updated);
                diagnosticsService.record("discovery", "Peer " + peer.displayName() + " -> " + newState);
            }
        }
    }
}
