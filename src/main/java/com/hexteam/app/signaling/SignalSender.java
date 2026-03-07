package com.hexteam.app.signaling;

import com.hexteam.app.discovery.PeerDescriptor;
import com.hexteam.app.metrics.DiagnosticsService;
import com.hexteam.app.security.NodeIdentityService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestClient;

import java.time.Clock;
import java.time.Instant;
import java.util.Map;
import java.util.UUID;

@Service
public class SignalSender {
    /**
     * Отправляет backend-to-backend сигналы на удалённый узел.
     */

    private static final Logger log = LoggerFactory.getLogger(SignalSender.class);

    private final RestClient restClient;
    private final NodeIdentityService identityService;
    private final DiagnosticsService diagnosticsService;
    private final Clock clock;

    public SignalSender(RestClient restClient,
                        NodeIdentityService identityService,
                        DiagnosticsService diagnosticsService,
                        Clock clock) {
        this.restClient = restClient;
        this.identityService = identityService;
        this.diagnosticsService = diagnosticsService;
        this.clock = clock;
    }

    public void sendInvite(PeerDescriptor peer, String sessionId) {
        SignalEnvelope envelope = new SignalEnvelope(
                SignalType.SESSION_INVITE,
                UUID.randomUUID().toString(),
                sessionId,
                identityService.currentIdentity().nodeId(),
                peer.nodeId(),
                Instant.now(clock),
                identityService.sign(sessionId + ":" + peer.nodeId()),
                Map.of("displayName", identityService.currentIdentity().displayName())
        );
        try {
            restClient.post()
                    .uri(peer.signalBaseUrl())
                    .body(envelope)
                    .retrieve()
                    .toBodilessEntity();
            diagnosticsService.record("signaling", "Отправлен invite sessionId=" + sessionId + " peer=" + peer.displayName());
        } catch (RuntimeException exception) {
            log.warn("Не удалось отправить signaling-сообщение peer {}", peer.nodeId(), exception);
            diagnosticsService.record("signaling", "Ошибка отправки invite: " + exception.getMessage());
        }
    }
}
