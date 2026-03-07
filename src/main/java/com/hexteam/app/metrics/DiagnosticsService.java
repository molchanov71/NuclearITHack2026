package com.hexteam.app.metrics;

import org.springframework.stereotype.Service;

import java.time.Instant;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.List;

@Service
public class DiagnosticsService {

    private static final int LIMIT = 200;

    private final Deque<DiagnosticEvent> events = new ArrayDeque<>();

    public synchronized void record(String category, String message) {
        if (events.size() >= LIMIT) {
            events.removeFirst();
        }
        events.addLast(new DiagnosticEvent(Instant.now(), category, message));
    }

    public synchronized List<DiagnosticEvent> recentEvents() {
        return List.copyOf(events);
    }
}
