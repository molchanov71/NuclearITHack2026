#pragma once

#include <QSqlDatabase>

#include "../model/domain_entities.hpp"

/**
 * @brief Базовый репозиторий peers в SQLite.
 */
class PeerRepository
{
public:
    explicit PeerRepository(const QSqlDatabase &database);
    bool save(const PeerDescriptor &peer) const;
    [[nodiscard]] QVector<PeerDescriptor> loadAll() const;

private:
    QSqlDatabase database_;
};

/**
 * @brief Базовый репозиторий сообщений в SQLite.
 */
class MessageRepository
{
public:
    explicit MessageRepository(const QSqlDatabase &database);
    bool save(const ChatMessage &message) const;
    [[nodiscard]] QVector<ChatMessage> loadAll() const;

private:
    QSqlDatabase database_;
};

/**
 * @brief Базовый репозиторий файловых передач в SQLite.
 */
class TransferRepository
{
public:
    explicit TransferRepository(const QSqlDatabase &database);
    bool save(const FileTransfer &transfer) const;
    [[nodiscard]] QVector<FileTransfer> loadAll() const;

private:
    QSqlDatabase database_;
};

/**
 * @brief Базовый репозиторий технических метрик в SQLite.
 */
class MetricsRepository
{
public:
    explicit MetricsRepository(const QSqlDatabase &database);
    bool save(const MetricSnapshot &snapshot) const;
    [[nodiscard]] QVector<MetricSnapshot> loadAll() const;

private:
    QSqlDatabase database_;
};

/**
 * @brief Базовый репозиторий файловых манифестов в SQLite.
 */
class FileManifestRepository
{
public:
    explicit FileManifestRepository(const QSqlDatabase &database);
    bool save(const FileManifest &manifest) const;
    [[nodiscard]] QVector<FileManifest> loadAll() const;

private:
    QSqlDatabase database_;
};
