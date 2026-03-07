package com.hexteam.app.session;

import java.time.Instant;

public record Session(
        String sessionId,
        String localNodeId,
        String remoteNodeId,
        String remoteDisplayName,
        SessionState state,
        Instant createdAt,
        Instant updatedAt
) {
}
