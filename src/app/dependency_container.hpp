#pragma once

#include <memory>

#include <spdlog/logger.h>

#include "app_config.hpp"
#include "../controller/controllers.hpp"
#include "../infrastructure/database.hpp"
#include "../infrastructure/identity_service.hpp"
#include "../infrastructure/log_service.hpp"
#include "../infrastructure/network_services.hpp"
#include "../infrastructure/protocol.hpp"
#include "../infrastructure/repositories.hpp"
#include "../infrastructure/runtime_paths.hpp"
#include "../model/stores.hpp"

/**
 * @brief Хранит общие зависимости приложения и управляет их жизненным циклом.
 */
class DependencyContainer
{
public:
    /** @brief Создаёт пустой контейнер. Реальные сервисы поднимаются в initialize(). */
    DependencyContainer() = default;

    /**
     * @brief Инициализирует общие сервисы на основе переданной конфигурации.
     * @param config Конфигурация приложения, подготовленная runtime.
     */
    void initialize(const AppConfig &config);

    /** @brief Освобождает общие сервисы и сбрасывает состояние контейнера. */
    void shutdown();

    /**
     * @brief Возвращает активную конфигурацию приложения.
     * @return Константная ссылка на конфигурацию, хранящуюся в контейнере.
     */
    [[nodiscard]] const AppConfig &config() const;

    /**
     * @brief Возвращает общий логгер приложения.
     * @return Экземпляр логгера, принадлежащий контейнеру.
     */
    [[nodiscard]] spdlog::logger &logger() const;
    [[nodiscard]] const UiLogBuffer &uiLogBuffer() const;
    [[nodiscard]] const RuntimePaths &paths() const;
    [[nodiscard]] const Database &database() const;
    [[nodiscard]] const PeerRepository &peerRepository() const;
    [[nodiscard]] const MessageRepository &messageRepository() const;
    [[nodiscard]] const TransferRepository &transferRepository() const;
    [[nodiscard]] const MetricsRepository &metricsRepository() const;
    [[nodiscard]] const FileManifestRepository &fileManifestRepository() const;
    [[nodiscard]] const IdentityService &identityService() const;
    [[nodiscard]] const MessageCodec &messageCodec() const;
    [[nodiscard]] NodeIdentityStore &nodeIdentityStore();
    [[nodiscard]] PeerRegistryModel &peerRegistry();
    [[nodiscard]] PeersTableModel &peersTableModel();
    [[nodiscard]] SessionStore &sessionStore();
    [[nodiscard]] MessageStore &messageStore();
    [[nodiscard]] TransferStore &transferStore();
    [[nodiscard]] MetricsStore &metricsStore();
    [[nodiscard]] TrustStoreModel &trustStore();
    [[nodiscard]] AppController &appController();
    [[nodiscard]] PeerController &peerController();
    [[nodiscard]] SessionController &sessionController();
    [[nodiscard]] ChatController &chatController();
    [[nodiscard]] FileTransferController &fileTransferController();
    [[nodiscard]] CallController &callController();
    [[nodiscard]] DiagnosticsController &diagnosticsController();

    /**
     * @brief Показывает, завершилась ли initialize() успешно.
     * @return true, если общие сервисы готовы к использованию.
     */
    [[nodiscard]] bool isInitialized() const;
    void refreshDiscoveryState();

private:
    void handlePeerUpdated(const PeerDescriptor &peer);

    /** @brief Сохранённая конфигурация приложения. */
    AppConfig config_{};
    /** @brief Подготовленные каталоги runtime. */
    RuntimePaths paths_{};
    /** @brief Сервис создания runtime-каталогов. */
    RuntimePathsService runtimePathsService_{};
    /** @brief Сервис логирования и UI-буфера логов. */
    LogService logService_{};
    /** @brief Общий экземпляр логгера runtime. */
    std::shared_ptr<spdlog::logger> logger_{};
    /** @brief SQLite база приложения. */
    Database database_{};
    /** @brief Репозиторий peers. */
    std::unique_ptr<PeerRepository> peerRepository_{};
    /** @brief Репозиторий сообщений. */
    std::unique_ptr<MessageRepository> messageRepository_{};
    /** @brief Репозиторий передач файлов. */
    std::unique_ptr<TransferRepository> transferRepository_{};
    /** @brief Репозиторий метрик. */
    std::unique_ptr<MetricsRepository> metricsRepository_{};
    /** @brief Репозиторий файловых манифестов. */
    std::unique_ptr<FileManifestRepository> fileManifestRepository_{};
    /** @brief Криптографический сервис локальной идентичности. */
    IdentityService identityService_{};
    /** @brief Единый codec control plane сообщений. */
    MessageCodec messageCodec_{};
    /** @brief Трекер дедупликации messageId. */
    AckTracker ackTracker_{};
    /** @brief UDP discovery сервис. */
    DiscoveryService discoveryService_{};
    /** @brief TCP control plane сервер. */
    ControlServer controlServer_{};
    /** @brief TCP файловый сервер. */
    FileChannelServer fileChannelServer_{};
    /** @brief UDP голосовой транспорт. */
    VoiceUdpTransport voiceUdpTransport_{};
    /** @brief Идентичность текущего узла. */
    NodeIdentityStore nodeIdentityStore_{};
    /** @brief Реестр известных peers. */
    PeerRegistryModel peerRegistry_{};
    /** @brief Табличная модель peers для страницы discovery. */
    PeersTableModel peersTableModel_{};
    /** @brief Хранилище сессий. */
    SessionStore sessionStore_{};
    /** @brief Хранилище сообщений. */
    MessageStore messageStore_{};
    /** @brief Хранилище передач файлов и их манифестов. */
    TransferStore transferStore_{};
    /** @brief Хранилище технических метрик. */
    MetricsStore metricsStore_{};
    /** @brief Хранилище доверия к peers. */
    TrustStoreModel trustStore_{};
    /** @brief Верхнеуровневый app-контроллер. */
    std::unique_ptr<AppController> appController_{};
    /** @brief Контроллер peers. */
    std::unique_ptr<PeerController> peerController_{};
    /** @brief Контроллер сессий. */
    std::unique_ptr<SessionController> sessionController_{};
    /** @brief Контроллер чата. */
    std::unique_ptr<ChatController> chatController_{};
    /** @brief Контроллер файловых передач. */
    std::unique_ptr<FileTransferController> fileTransferController_{};
    /** @brief Контроллер звонков. */
    std::unique_ptr<CallController> callController_{};
    /** @brief Контроллер диагностики. */
    std::unique_ptr<DiagnosticsController> diagnosticsController_{};
    /** @brief Флаг, показывающий, что контейнер содержит активные сервисы. */
    bool initialized_ = false;
};
