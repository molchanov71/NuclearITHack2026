#pragma once

#include <functional>

#include <QStringList>

#include "../model/stores.hpp"

class AppConfig;

/**
 * @brief Агрегирует общее состояние приложения для верхнего уровня UI.
 */
class AppController
{
public:
    /**
     * @brief Создаёт контроллер поверх общей конфигурации и stores.
     * @param config Конфигурация приложения.
     * @param metricsStore Хранилище технических метрик.
     */
    AppController(const AppConfig &config, const MetricsStore &metricsStore);

    /**
     * @brief Формирует краткую техническую сводку по узлу.
     * @return Список строк для верхней панели приложения.
     */
    [[nodiscard]] QStringList summaryLines() const;

private:
    const AppConfig &config_;
    const MetricsStore &metricsStore_;
};

/**
 * @brief Подготавливает представление реестра известных peers для UI.
 */
class PeerController
{
public:
    PeerController(PeerRegistryModel &peerRegistry,
                   PeersTableModel &tableModel,
                   const AppConfig &config,
                   std::function<void(const PeerDescriptor &)> onPeerChanged = {});
    [[nodiscard]] QStringList peerLines() const;
    [[nodiscard]] PeersTableModel &tableModel() const;
    void refresh();
    void addManualPeer(const QString &address);

private:
    PeerRegistryModel &peerRegistry_;
    PeersTableModel &tableModel_;
    const AppConfig &config_;
    std::function<void(const PeerDescriptor &)> onPeerChanged_{};
};

/**
 * @brief Подготавливает представление сессий для UI.
 */
class SessionController
{
public:
    explicit SessionController(const SessionStore &sessionStore);
    [[nodiscard]] QStringList sessionLines() const;

private:
    const SessionStore &sessionStore_;
};

/**
 * @brief Подготавливает историю сообщений для UI.
 */
class ChatController
{
public:
    explicit ChatController(const MessageStore &messageStore);
    [[nodiscard]] QStringList chatLines() const;

private:
    const MessageStore &messageStore_;
};

/**
 * @brief Подготавливает список файловых передач для UI.
 */
class FileTransferController
{
public:
    explicit FileTransferController(const TransferStore &transferStore);
    [[nodiscard]] QStringList transferLines() const;

private:
    const TransferStore &transferStore_;
};

/**
 * @brief Подготавливает представление состояния звонков для UI.
 */
class CallController
{
public:
    explicit CallController(const MetricsStore &metricsStore);
    [[nodiscard]] QStringList callLines() const;

private:
    const MetricsStore &metricsStore_;
};

/**
 * @brief Подготавливает диагностические строки по runtime и trust store.
 */
class DiagnosticsController
{
public:
    DiagnosticsController(const NodeIdentityStore &identityStore,
                          const MetricsStore &metricsStore,
                          const TrustStoreModel &trustStore);

    [[nodiscard]] QStringList diagnosticLines() const;

private:
    const NodeIdentityStore &identityStore_;
    const MetricsStore &metricsStore_;
    const TrustStoreModel &trustStore_;
};
