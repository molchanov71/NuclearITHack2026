package com.hexteam.app.signaling;

import java.time.Instant;
import java.util.Map;

public record SignalEnvelope(
        SignalType type,
        String messageId,
        String sessionId,
        String senderNodeId,
        String targetNodeId,
        Instant createdAt,
        String signature,
        Map<String, Object> payload
) {
}
