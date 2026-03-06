#pragma once

#include <QByteArray>
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QVector>

#include "domain_enums.hpp"

/**
 * @brief Идентичность текущего узла приложения.
 */
struct NodeIdentity
{
    QString nodeId;
    QString displayName;
    QString deviceName;
    QByteArray publicKey;
    QDateTime createdAt;
};

/**
 * @brief Базовое описание удалённого peer.
 */
struct PeerDescriptor
{
    QString peerId;
    QString displayName;
    QStringList addresses;
    QDateTime lastSeenAt;
    PeerStatus status = PeerStatus::Offline;
    TrustLevel trustLevel = TrustLevel::Unknown;
};

/**
 * @brief Активная или историческая сессия между узлами.
 */
struct Session
{
    QString sessionId;
    QString peerId;
    SessionStatus status = SessionStatus::Closed;
    QDateTime connectedAt;
    QDateTime lastActivityAt;
    bool encrypted = false;
};

/**
 * @brief Сообщение чата.
 */
struct ChatMessage
{
    QString messageId;
    QString peerId;
    QString sessionId;
    QString authorId;
    QString text;
    QDateTime createdAt;
    MessageDirection direction = MessageDirection::Incoming;
    MessageDeliveryStatus deliveryStatus = MessageDeliveryStatus::Pending;
};

/**
 * @brief Метаданные файла для передачи по сети.
 */
struct FileManifest
{
    QString manifestId;
    QString fileName;
    QString relativePath;
    QByteArray sha256;
    qint64 totalSize = 0;
    qint64 chunkSize = 0;
    int chunkCount = 0;
};

/**
 * @brief Состояние передачи файла.
 */
struct FileTransfer
{
    QString transferId;
    QString peerId;
    QString manifestId;
    TransferStatus status = TransferStatus::Pending;
    qint64 bytesTransferred = 0;
    qint64 bytesTotal = 0;
    QDateTime updatedAt;
};

/**
 * @brief Состояние голосового вызова.
 */
struct CallState
{
    QString callId;
    QString peerId;
    CallStatus status = CallStatus::Invited;
    QDateTime startedAt;
    int bitrate = 0;
    double packetLoss = 0.0;
};

/**
 * @brief Снимок технических метрик для peer или сессии.
 */
struct MetricSnapshot
{
    QString peerId;
    QString sessionId;
    QDateTime capturedAt;
    int latencyMs = 0;
    int jitterMs = 0;
    double packetLoss = 0.0;
    int throughputKbps = 0;
};

/**
 * @brief Маршрут или сетевой адрес для peer.
 */
struct RouteEntry
{
    QString peerId;
    QString address;
    quint16 port = 0;
    QString transport;
    bool direct = true;
};
