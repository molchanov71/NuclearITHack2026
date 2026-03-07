package com.hexteam.app.config;

import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.validation.annotation.Validated;

@Validated
@ConfigurationProperties(prefix = "hex")
public class HexProperties {
    /**
     * Централизованный конфиг приложения.
     * Содержит только те настройки, которые нужны для работы узла,
     * а не для инфраструктуры Spring Boot в целом.
     */

    private final Discovery discovery = new Discovery();
    private final Signal signal = new Signal();
    private final Storage storage = new Storage();
    private final Files files = new Files();
    private final Metrics metrics = new Metrics();

    public Discovery getDiscovery() {
        return discovery;
    }

    public Signal getSignal() {
        return signal;
    }

    public Storage getStorage() {
        return storage;
    }

    public Files getFiles() {
        return files;
    }

    public Metrics getMetrics() {
        return metrics;
    }

    public static class Discovery {
        /**
         * Discovery можно отключать в тестах, чтобы не поднимать UDP listener.
         */

        private boolean enabled = true;

        @Min(1)
        private int port = 49090;

        @Min(100)
        private long intervalMs = 3000;

        @Min(1000)
        private long ttlMs = 10000;

        public boolean isEnabled() {
            return enabled;
        }

        public void setEnabled(boolean enabled) {
            this.enabled = enabled;
        }

        public int getPort() {
            return port;
        }

        public void setPort(int port) {
            this.port = port;
        }

        public long getIntervalMs() {
            return intervalMs;
        }

        public void setIntervalMs(long intervalMs) {
            this.intervalMs = intervalMs;
        }

        public long getTtlMs() {
            return ttlMs;
        }

        public void setTtlMs(long ttlMs) {
            this.ttlMs = ttlMs;
        }
    }

    public static class Signal {
        /**
         * Базовый путь для backend-to-backend signaling API.
         */

        @NotBlank
        private String basePath = "/api/signal";

        public String getBasePath() {
            return basePath;
        }

        public void setBasePath(String basePath) {
            this.basePath = basePath;
        }
    }

    public static class Storage {
        /**
         * Корневая директория локального состояния узла.
         */

        @NotBlank
        private String root = "./data";

        public String getRoot() {
            return root;
        }

        public void setRoot(String root) {
            this.root = root;
        }
    }

    public static class Files {

        @NotBlank
        private String tempDir = "./data/files/tmp";

        @NotBlank
        private String finalDir = "./data/files/final";

        public String getTempDir() {
            return tempDir;
        }

        public void setTempDir(String tempDir) {
            this.tempDir = tempDir;
        }

        public String getFinalDir() {
            return finalDir;
        }

        public void setFinalDir(String finalDir) {
            this.finalDir = finalDir;
        }
    }

    public static class Metrics {

        private boolean enabled = true;

        public boolean isEnabled() {
            return enabled;
        }

        public void setEnabled(boolean enabled) {
            this.enabled = enabled;
        }
    }
}
