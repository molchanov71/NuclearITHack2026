package com.hexteam.app.metrics;

import java.time.Instant;

public record DiagnosticEvent(
        Instant timestamp,
        String category,
        String message
) {
}
