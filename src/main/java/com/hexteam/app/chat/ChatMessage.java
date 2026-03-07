package com.hexteam.app.chat;

import java.time.Instant;

public record ChatMessage(
        String id,
        String sessionId,
        String senderNodeId,
        String targetNodeId,
        String content,
        MessageStatus status,
        Instant createdAt
) {
}
