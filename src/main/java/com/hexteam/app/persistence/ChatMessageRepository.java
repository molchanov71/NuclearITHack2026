package com.hexteam.app.persistence;

import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface ChatMessageRepository extends JpaRepository<ChatMessageEntity, String> {

    List<ChatMessageEntity> findTop50ByOrderByCreatedAtDesc();
}
