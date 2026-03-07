#include "repositories.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QSqlQuery>

namespace
{
QString toIso(const QDateTime &value)
{
    return value.toUTC().toString(Qt::ISODateWithMs);
}

QDateTime fromIso(const QVariant &value)
{
    return QDateTime::fromString(value.toString(), Qt::ISODate);
}

QByteArray encodeStringList(const QStringList &values)
{
    QJsonArray array;
    for (const QString &value : values) {
        array.append(value);
    }

    return QJsonDocument(array).toJson(QJsonDocument::Compact);
}

QStringList decodeAddresses(const QVariant &value)
{
    QStringList addresses;
    const QJsonArray array = QJsonDocument::fromJson(value.toByteArray()).array();
    for (const QJsonValue &entry : array) {
        addresses.append(entry.toString());
    }
    return addresses;
}

QStringList decodeStringList(const QVariant &value)
{
    QStringList values;
    const QJsonArray array = QJsonDocument::fromJson(value.toByteArray()).array();
    for (const QJsonValue &entry : array) {
        values.append(entry.toString());
    }
    return values;
}
}

PeerRepository::PeerRepository(const QSqlDatabase &database)
    : database_(database)
{
}

bool PeerRepository::save(const PeerDescriptor &peer) const
{
    QSqlQuery query(database_);
    query.prepare("INSERT OR REPLACE INTO peers(peer_id, display_name, addresses, capabilities, discovery_port, control_port, file_port, voice_port, last_seen_at, status, trust_level) "
                  "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(peer.peerId);
    query.addBindValue(peer.displayName);
    query.addBindValue(encodeStringList(peer.addresses));
    query.addBindValue(encodeStringList(peer.capabilities));
    query.addBindValue(peer.discoveryPort);
    query.addBindValue(peer.controlPort);
    query.addBindValue(peer.filePort);
    query.addBindValue(peer.voicePort);
    query.addBindValue(toIso(peer.lastSeenAt));
    query.addBindValue(static_cast<int>(peer.status));
    query.addBindValue(static_cast<int>(peer.trustLevel));
    return query.exec();
}

QVector<PeerDescriptor> PeerRepository::loadAll() const
{
    QVector<PeerDescriptor> peers;
    QSqlQuery query(QStringLiteral("SELECT peer_id, display_name, addresses, capabilities, discovery_port, control_port, file_port, voice_port, last_seen_at, status, trust_level FROM peers"),
                    database_);
    while (query.next()) {
        peers.append(PeerDescriptor{
                .peerId = query.value(0).toString(),
                .displayName = query.value(1).toString(),
                .addresses = decodeAddresses(query.value(2)),
                .capabilities = decodeStringList(query.value(3)),
                .discoveryPort = static_cast<quint16>(query.value(4).toUInt()),
                .controlPort = static_cast<quint16>(query.value(5).toUInt()),
                .filePort = static_cast<quint16>(query.value(6).toUInt()),
                .voicePort = static_cast<quint16>(query.value(7).toUInt()),
                .lastSeenAt = fromIso(query.value(8)),
                .status = static_cast<PeerStatus>(query.value(9).toInt()),
                .trustLevel = static_cast<TrustLevel>(query.value(10).toInt()),
        });
    }
    return peers;
}

MessageRepository::MessageRepository(const QSqlDatabase &database)
    : database_(database)
{
}

bool MessageRepository::save(const ChatMessage &message) const
{
    QSqlQuery query(database_);
    query.prepare("INSERT OR REPLACE INTO messages(message_id, peer_id, session_id, author_id, text, created_at, direction, delivery_status) "
                  "VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(message.messageId);
    query.addBindValue(message.peerId);
    query.addBindValue(message.sessionId);
    query.addBindValue(message.authorId);
    query.addBindValue(message.text);
    query.addBindValue(toIso(message.createdAt));
    query.addBindValue(static_cast<int>(message.direction));
    query.addBindValue(static_cast<int>(message.deliveryStatus));
    return query.exec();
}

QVector<ChatMessage> MessageRepository::loadAll() const
{
    QVector<ChatMessage> messages;
    QSqlQuery query(QStringLiteral("SELECT message_id, peer_id, session_id, author_id, text, created_at, direction, delivery_status FROM messages"),
                    database_);
    while (query.next()) {
        messages.append(ChatMessage{
                .messageId = query.value(0).toString(),
                .peerId = query.value(1).toString(),
                .sessionId = query.value(2).toString(),
                .authorId = query.value(3).toString(),
                .text = query.value(4).toString(),
                .createdAt = fromIso(query.value(5)),
                .direction = static_cast<MessageDirection>(query.value(6).toInt()),
                .deliveryStatus = static_cast<MessageDeliveryStatus>(query.value(7).toInt()),
        });
    }
    return messages;
}

TransferRepository::TransferRepository(const QSqlDatabase &database)
    : database_(database)
{
}

bool TransferRepository::save(const FileTransfer &transfer) const
{
    QSqlQuery query(database_);
    query.prepare("INSERT OR REPLACE INTO file_transfers(transfer_id, peer_id, manifest_id, status, bytes_transferred, bytes_total, updated_at) "
                  "VALUES(?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(transfer.transferId);
    query.addBindValue(transfer.peerId);
    query.addBindValue(transfer.manifestId);
    query.addBindValue(static_cast<int>(transfer.status));
    query.addBindValue(transfer.bytesTransferred);
    query.addBindValue(transfer.bytesTotal);
    query.addBindValue(toIso(transfer.updatedAt));
    return query.exec();
}

QVector<FileTransfer> TransferRepository::loadAll() const
{
    QVector<FileTransfer> transfers;
    QSqlQuery query(QStringLiteral("SELECT transfer_id, peer_id, manifest_id, status, bytes_transferred, bytes_total, updated_at FROM file_transfers"),
                    database_);
    while (query.next()) {
        transfers.append(FileTransfer{
                .transferId = query.value(0).toString(),
                .peerId = query.value(1).toString(),
                .manifestId = query.value(2).toString(),
                .status = static_cast<TransferStatus>(query.value(3).toInt()),
                .bytesTransferred = query.value(4).toLongLong(),
                .bytesTotal = query.value(5).toLongLong(),
                .updatedAt = fromIso(query.value(6)),
        });
    }
    return transfers;
}

MetricsRepository::MetricsRepository(const QSqlDatabase &database)
    : database_(database)
{
}

bool MetricsRepository::save(const MetricSnapshot &snapshot) const
{
    QSqlQuery query(database_);
    query.prepare("INSERT INTO metrics_snapshots(peer_id, session_id, captured_at, latency_ms, jitter_ms, packet_loss, throughput_kbps) "
                  "VALUES(?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(snapshot.peerId);
    query.addBindValue(snapshot.sessionId);
    query.addBindValue(toIso(snapshot.capturedAt));
    query.addBindValue(snapshot.latencyMs);
    query.addBindValue(snapshot.jitterMs);
    query.addBindValue(snapshot.packetLoss);
    query.addBindValue(snapshot.throughputKbps);
    return query.exec();
}

QVector<MetricSnapshot> MetricsRepository::loadAll() const
{
    QVector<MetricSnapshot> snapshots;
    QSqlQuery query(QStringLiteral("SELECT peer_id, session_id, captured_at, latency_ms, jitter_ms, packet_loss, throughput_kbps FROM metrics_snapshots"),
                    database_);
    while (query.next()) {
        snapshots.append(MetricSnapshot{
                .peerId = query.value(0).toString(),
                .sessionId = query.value(1).toString(),
                .capturedAt = fromIso(query.value(2)),
                .latencyMs = query.value(3).toInt(),
                .jitterMs = query.value(4).toInt(),
                .packetLoss = query.value(5).toDouble(),
                .throughputKbps = query.value(6).toInt(),
        });
    }
    return snapshots;
}

FileManifestRepository::FileManifestRepository(const QSqlDatabase &database)
    : database_(database)
{
}

bool FileManifestRepository::save(const FileManifest &manifest) const
{
    QSqlQuery query(database_);
    query.prepare("INSERT OR REPLACE INTO file_manifests(manifest_id, file_name, relative_path, sha256, total_size, chunk_size, chunk_count) "
                  "VALUES(?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(manifest.manifestId);
    query.addBindValue(manifest.fileName);
    query.addBindValue(manifest.relativePath);
    query.addBindValue(manifest.sha256);
    query.addBindValue(manifest.totalSize);
    query.addBindValue(manifest.chunkSize);
    query.addBindValue(manifest.chunkCount);
    return query.exec();
}

QVector<FileManifest> FileManifestRepository::loadAll() const
{
    QVector<FileManifest> manifests;
    QSqlQuery query(QStringLiteral("SELECT manifest_id, file_name, relative_path, sha256, total_size, chunk_size, chunk_count FROM file_manifests"),
                    database_);
    while (query.next()) {
        manifests.append(FileManifest{
                .manifestId = query.value(0).toString(),
                .fileName = query.value(1).toString(),
                .relativePath = query.value(2).toString(),
                .sha256 = query.value(3).toByteArray(),
                .totalSize = query.value(4).toLongLong(),
                .chunkSize = query.value(5).toLongLong(),
                .chunkCount = query.value(6).toInt(),
        });
    }
    return manifests;
}
