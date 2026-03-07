package com.hexteam.app.session;

import com.hexteam.app.discovery.PeerDescriptor;
import com.hexteam.app.discovery.PeerRegistry;
import com.hexteam.app.metrics.DiagnosticsService;
import com.hexteam.app.security.NodeIdentityService;
import com.hexteam.app.signaling.SignalEnvelope;
import com.hexteam.app.signaling.SignalSender;
import com.hexteam.app.signaling.SignalType;
import org.springframework.stereotype.Service;

import java.time.Clock;
import java.time.Instant;
import java.util.Collection;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;

@Service
public class SessionCoordinator {
    /**
     * Координирует жизненный цикл логических сессий до появления
     * полноценной transport/WebRTC-логики.
     */

    private final Map<String, Session> sessions = new ConcurrentHashMap<>();
    private final PeerRegistry peerRegistry;
    private final SignalSender signalSender;
    private final NodeIdentityService identityService;
    private final DiagnosticsService diagnosticsService;
    private final Clock clock;

    public SessionCoordinator(PeerRegistry peerRegistry,
                              SignalSender signalSender,
                              NodeIdentityService identityService,
                              DiagnosticsService diagnosticsService,
                              Clock clock) {
        this.peerRegistry = peerRegistry;
        this.signalSender = signalSender;
        this.identityService = identityService;
        this.diagnosticsService = diagnosticsService;
        this.clock = clock;
    }

    /**
     * Инициирует новую сессию с выбранным peer-ом и отправляет invite.
     */
    public Session startSession(String peerNodeId) {
        PeerDescriptor peer = peerRegistry.findByNodeId(peerNodeId);
        if (peer == null) {
            throw new IllegalArgumentException("Peer не найден: " + peerNodeId);
        }
        Instant now = Instant.now(clock);
        String sessionId = UUID.randomUUID().toString();
        Session session = new Session(
                sessionId,
                identityService.currentIdentity().nodeId(),
                peer.nodeId(),
                peer.displayName(),
                SessionState.NEW,
                now,
                now
        );
        sessions.put(sessionId, session);
        transition(sessionId, SessionState.SIGNALING);
        signalSender.sendInvite(peer, sessionId);
        return sessions.get(sessionId);
    }

    /**
     * Обрабатывает входящие signaling-сообщения и меняет состояние сессии.
     */
    public void handleIncoming(SignalEnvelope envelope) {
        switch (envelope.type()) {
            case SESSION_INVITE -> registerIncomingInvite(envelope);
            case SESSION_ACCEPT -> transition(envelope.sessionId(), SessionState.CONNECTING);
            case RTC_OFFER, RTC_ANSWER, ICE_CANDIDATE -> transition(envelope.sessionId(), SessionState.CONNECTING);
            case CALL_END, ERROR -> transition(envelope.sessionId(), SessionState.CLOSED);
            case ACK, CALL_START -> diagnosticsService.record("signaling", "Обработан сигнал " + envelope.type());
        }
    }

    public Collection<Session> allSessions() {
        return sessions.values().stream()
                .sorted((left, right) -> right.updatedAt().compareTo(left.updatedAt()))
                .toList();
    }

    public void transition(String sessionId, SessionState newState) {
        Session current = sessions.get(sessionId);
        if (current == null) {
            return;
        }
        Session updated = new Session(
                current.sessionId(),
                current.localNodeId(),
                current.remoteNodeId(),
                current.remoteDisplayName(),
                newState,
                current.createdAt(),
                Instant.now(clock)
        );
        sessions.put(sessionId, updated);
        diagnosticsService.record("session", "Сессия " + sessionId + " -> " + newState);
    }

    private void registerIncomingInvite(SignalEnvelope envelope) {
        Instant now = Instant.now(clock);
        Session session = new Session(
                envelope.sessionId(),
                identityService.currentIdentity().nodeId(),
                envelope.senderNodeId(),
                String.valueOf(envelope.payload().getOrDefault("displayName", envelope.senderNodeId())),
                SessionState.INVITED,
                now,
                now
        );
        sessions.put(envelope.sessionId(), session);
        diagnosticsService.record("session", "Получено приглашение на сессию " + envelope.sessionId());
    }
}
