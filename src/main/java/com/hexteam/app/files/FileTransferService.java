package com.hexteam.app.files;

import com.hexteam.app.persistence.FileTransferEntity;
import com.hexteam.app.persistence.FileTransferRepository;
import org.springframework.stereotype.Service;

import java.time.Clock;
import java.time.Instant;
import java.util.List;

@Service
public class FileTransferService {

    private final FileTransferRepository repository;
    private final Clock clock;

    public FileTransferService(FileTransferRepository repository, Clock clock) {
        this.repository = repository;
        this.clock = clock;
    }

    public List<FileTransfer> recentTransfers() {
        return repository.findTop50ByOrderByCreatedAtDesc().stream()
                .map(this::toDomain)
                .toList();
    }

    public FileTransfer createPlaceholder(String sessionId, String fileName, long fileSize) {
        FileTransferEntity entity = new FileTransferEntity();
        entity.setId(java.util.UUID.randomUUID().toString());
        entity.setSessionId(sessionId);
        entity.setFileName(fileName);
        entity.setFileSize(fileSize);
        entity.setStatus(TransferStatus.OFFERED);
        entity.setCreatedAt(Instant.now(clock));
        entity.setProgressPercent(0);
        repository.save(entity);
        return toDomain(entity);
    }

    private FileTransfer toDomain(FileTransferEntity entity) {
        return new FileTransfer(
                entity.getId(),
                entity.getSessionId(),
                entity.getFileName(),
                entity.getFileSize(),
                entity.getStatus(),
                entity.getCreatedAt(),
                entity.getProgressPercent()
        );
    }
}
