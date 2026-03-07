package com.hexteam.app.discovery;

import com.hexteam.app.security.TrustState;

import java.time.Instant;
import java.util.Set;

public record PeerDescriptor(
        String nodeId,
        String displayName,
        String protocolVersion,
        String signalBaseUrl,
        String publicKeyFingerprint,
        Set<String> capabilities,
        long seq,
        Instant lastSeen,
        Instant announcedAt,
        PeerState state,
        TrustState trustState
) {
}
