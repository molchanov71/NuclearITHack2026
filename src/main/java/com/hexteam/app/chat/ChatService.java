package com.hexteam.app.chat;

import com.hexteam.app.persistence.ChatMessageEntity;
import com.hexteam.app.persistence.ChatMessageRepository;
import org.springframework.stereotype.Service;

import java.time.Clock;
import java.time.Instant;
import java.util.List;
import java.util.UUID;

@Service
public class ChatService {
    /**
     * Пока сервис хранит черновики и локальную историю,
     * но интерфейс уже повторяет будущий сценарий реального чата.
     */

    private final ChatMessageRepository repository;
    private final Clock clock;

    public ChatService(ChatMessageRepository repository, Clock clock) {
        this.repository = repository;
        this.clock = clock;
    }

    public ChatMessage savePendingMessage(String sessionId, String senderNodeId, String targetNodeId, String content) {
        ChatMessageEntity entity = new ChatMessageEntity();
        entity.setId(UUID.randomUUID().toString());
        entity.setSessionId(sessionId);
        entity.setSenderNodeId(senderNodeId);
        entity.setTargetNodeId(targetNodeId);
        entity.setContent(content);
        entity.setStatus(MessageStatus.PENDING);
        entity.setCreatedAt(Instant.now(clock));
        repository.save(entity);
        return toDomain(entity);
    }

    public List<ChatMessage> recentMessages() {
        return repository.findTop50ByOrderByCreatedAtDesc()
                .stream()
                .map(this::toDomain)
                .toList();
    }

    private ChatMessage toDomain(ChatMessageEntity entity) {
        return new ChatMessage(
                entity.getId(),
                entity.getSessionId(),
                entity.getSenderNodeId(),
                entity.getTargetNodeId(),
                entity.getContent(),
                entity.getStatus(),
                entity.getCreatedAt()
        );
    }
}
