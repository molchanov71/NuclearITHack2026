package com.hexteam.app.discovery;

import java.time.Instant;
import java.util.Set;

public record DiscoveryAnnounce(
        String nodeId,
        String displayName,
        String protocolVersion,
        String signalBaseUrl,
        String publicKeyFingerprint,
        Set<String> capabilities,
        Instant timestamp,
        long seq
) {
}
