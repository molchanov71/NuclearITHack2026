package com.hexteam.app.signaling;

import com.hexteam.app.metrics.DiagnosticsService;
import com.hexteam.app.session.SessionCoordinator;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.Map;

@RestController
@RequestMapping("${hex.signal.base-path:/api/signal}")
public class SignalController {

    private final SessionCoordinator sessionCoordinator;
    private final DiagnosticsService diagnosticsService;

    public SignalController(SessionCoordinator sessionCoordinator, DiagnosticsService diagnosticsService) {
        this.sessionCoordinator = sessionCoordinator;
        this.diagnosticsService = diagnosticsService;
    }

    @PostMapping
    public ResponseEntity<Map<String, String>> receive(@RequestBody SignalEnvelope envelope) {
        sessionCoordinator.handleIncoming(envelope);
        diagnosticsService.record("signaling", "Получен сигнал " + envelope.type() + " для sessionId=" + envelope.sessionId());
        return ResponseEntity.accepted().body(Map.of("status", "accepted"));
    }
}
