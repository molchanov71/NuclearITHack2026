package com.hexteam.app.metrics;

import java.time.Instant;

public record MetricSnapshot(
        String sessionId,
        double rttMs,
        double jitterMs,
        double packetLossPercent,
        double bitrateKbps,
        long bufferedAmountBytes,
        Instant collectedAt
) {
}
