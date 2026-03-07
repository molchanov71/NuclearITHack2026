#pragma once

#include <QAbstractTableModel>
#include <QHash>
#include <QString>
#include <QVector>

#include "domain_entities.hpp"

/**
 * @brief Хранит идентичность текущего узла приложения.
 */
class NodeIdentityStore
{
public:
    void setIdentity(const NodeIdentity &identity);
    [[nodiscard]] const NodeIdentity &identity() const;
    [[nodiscard]] bool hasIdentity() const;

private:
    NodeIdentity identity_{};
    bool hasIdentity_ = false;
};

/**
 * @brief Хранит и обновляет список известных peers.
 */
class PeerRegistryModel
{
public:
    void upsert(const PeerDescriptor &peer);
    bool updateStatus(const QString &peerId, PeerStatus status);
    bool remove(const QString &peerId);
    [[nodiscard]] bool contains(const QString &peerId) const;
    [[nodiscard]] const PeerDescriptor *find(const QString &peerId) const;
    [[nodiscard]] QVector<PeerDescriptor> all() const;
    [[nodiscard]] qsizetype size() const;

private:
    QHash<QString, PeerDescriptor> peers_{};
};

/**
 * @brief Табличная модель peers для страницы discovery.
 */
class PeersTableModel final : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit PeersTableModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    /** @brief Обновляет снимок таблицы на основе реестра peers. */
    void refreshFromRegistry(const PeerRegistryModel &peerRegistry);
    /** @brief Возвращает peer по строке таблицы. */
    [[nodiscard]] const PeerDescriptor *peerAt(int row) const;

private:
    [[nodiscard]] QString formatStatus(PeerStatus status) const;
    [[nodiscard]] QString formatEndpoints(const PeerDescriptor &peer) const;

    QVector<PeerDescriptor> peers_{};
};

/**
 * @brief Хранит сетевые сессии по идентификатору.
 */
class SessionStore
{
public:
    void upsert(const Session &session);
    bool remove(const QString &sessionId);
    [[nodiscard]] const Session *find(const QString &sessionId) const;
    [[nodiscard]] QVector<Session> all() const;
    [[nodiscard]] qsizetype size() const;

private:
    QHash<QString, Session> sessions_{};
};

/**
 * @brief Хранит сообщения чата в памяти.
 */
class MessageStore
{
public:
    void append(const ChatMessage &message);
    [[nodiscard]] QVector<ChatMessage> messagesForPeer(const QString &peerId) const;
    [[nodiscard]] QVector<ChatMessage> all() const;
    [[nodiscard]] qsizetype size() const;

private:
    QVector<ChatMessage> messages_{};
};

/**
 * @brief Хранит активные и завершённые передачи файлов.
 */
class TransferStore
{
public:
    void upsertTransfer(const FileTransfer &transfer);
    void upsertManifest(const FileManifest &manifest);
    bool removeTransfer(const QString &transferId);
    [[nodiscard]] const FileTransfer *findTransfer(const QString &transferId) const;
    [[nodiscard]] const FileManifest *findManifest(const QString &manifestId) const;
    [[nodiscard]] QVector<FileTransfer> transfers() const;
    [[nodiscard]] QVector<FileManifest> manifests() const;

private:
    QHash<QString, FileTransfer> transfers_{};
    QHash<QString, FileManifest> manifests_{};
};

/**
 * @brief Хранит последние технические метрики.
 */
class MetricsStore
{
public:
    void addSnapshot(const MetricSnapshot &snapshot);
    [[nodiscard]] QVector<MetricSnapshot> snapshotsForPeer(const QString &peerId) const;
    [[nodiscard]] QVector<MetricSnapshot> all() const;
    [[nodiscard]] qsizetype size() const;

private:
    QVector<MetricSnapshot> snapshots_{};
};

/**
 * @brief Хранит уровень доверия для peers.
 */
class TrustStoreModel
{
public:
    void setTrustLevel(const QString &peerId, TrustLevel level);
    [[nodiscard]] TrustLevel trustLevel(const QString &peerId) const;
    [[nodiscard]] QHash<QString, TrustLevel> all() const;
    [[nodiscard]] qsizetype size() const;

private:
    QHash<QString, TrustLevel> trustLevels_{};
};
