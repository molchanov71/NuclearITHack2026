package com.hexteam.app.files;

import java.time.Instant;

public record FileTransfer(
        String id,
        String sessionId,
        String fileName,
        long fileSize,
        TransferStatus status,
        Instant createdAt,
        int progressPercent
) {
}
