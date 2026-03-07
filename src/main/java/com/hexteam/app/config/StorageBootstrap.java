package com.hexteam.app.config;

import jakarta.annotation.PostConstruct;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

@Component
public class StorageBootstrap {
    /**
     * Подготавливает каталог хранения заранее, чтобы остальные сервисы
     * не занимались созданием директорий в рабочем коде.
     */

    private static final Logger log = LoggerFactory.getLogger(StorageBootstrap.class);

    private final HexProperties properties;

    public StorageBootstrap(HexProperties properties) {
        this.properties = properties;
    }

    @PostConstruct
    public void initializeDirectories() throws IOException {
        createDirectory(Path.of(properties.getStorage().getRoot()));
        createDirectory(Path.of(properties.getFiles().getTempDir()));
        createDirectory(Path.of(properties.getFiles().getFinalDir()));
        createDirectory(Path.of(properties.getStorage().getRoot(), "identity"));
        createDirectory(Path.of(properties.getStorage().getRoot(), "manifests"));
        log.info("Подготовлены директории локального хранения");
    }

    private void createDirectory(Path path) throws IOException {
        Files.createDirectories(path);
    }
}
