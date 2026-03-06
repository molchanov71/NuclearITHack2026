#include "stores.hpp"

void NodeIdentityStore::setIdentity(const NodeIdentity &identity)
{
    identity_ = identity;
    hasIdentity_ = true;
}

const NodeIdentity &NodeIdentityStore::identity() const
{
    return identity_;
}

bool NodeIdentityStore::hasIdentity() const
{
    return hasIdentity_;
}

void PeerRegistryModel::upsert(const PeerDescriptor &peer)
{
    peers_.insert(peer.peerId, peer);
}

bool PeerRegistryModel::remove(const QString &peerId)
{
    return peers_.remove(peerId) > 0;
}

bool PeerRegistryModel::contains(const QString &peerId) const
{
    return peers_.contains(peerId);
}

const PeerDescriptor *PeerRegistryModel::find(const QString &peerId) const
{
    const auto it = peers_.constFind(peerId);
    if (it == peers_.cend()) {
        return nullptr;
    }

    return &it.value();
}

QVector<PeerDescriptor> PeerRegistryModel::all() const
{
    return peers_.values().toVector();
}

qsizetype PeerRegistryModel::size() const
{
    return peers_.size();
}

void SessionStore::upsert(const Session &session)
{
    sessions_.insert(session.sessionId, session);
}

bool SessionStore::remove(const QString &sessionId)
{
    return sessions_.remove(sessionId) > 0;
}

const Session *SessionStore::find(const QString &sessionId) const
{
    const auto it = sessions_.constFind(sessionId);
    if (it == sessions_.cend()) {
        return nullptr;
    }

    return &it.value();
}

QVector<Session> SessionStore::all() const
{
    return sessions_.values().toVector();
}

qsizetype SessionStore::size() const
{
    return sessions_.size();
}

void MessageStore::append(const ChatMessage &message)
{
    messages_.append(message);
}

QVector<ChatMessage> MessageStore::messagesForPeer(const QString &peerId) const
{
    QVector<ChatMessage> filtered;
    filtered.reserve(messages_.size());

    for (const ChatMessage &message : messages_) {
        if (message.peerId == peerId) {
            filtered.append(message);
        }
    }

    return filtered;
}

QVector<ChatMessage> MessageStore::all() const
{
    return messages_;
}

qsizetype MessageStore::size() const
{
    return messages_.size();
}

void TransferStore::upsertTransfer(const FileTransfer &transfer)
{
    transfers_.insert(transfer.transferId, transfer);
}

void TransferStore::upsertManifest(const FileManifest &manifest)
{
    manifests_.insert(manifest.manifestId, manifest);
}

bool TransferStore::removeTransfer(const QString &transferId)
{
    return transfers_.remove(transferId) > 0;
}

const FileTransfer *TransferStore::findTransfer(const QString &transferId) const
{
    const auto it = transfers_.constFind(transferId);
    if (it == transfers_.cend()) {
        return nullptr;
    }

    return &it.value();
}

const FileManifest *TransferStore::findManifest(const QString &manifestId) const
{
    const auto it = manifests_.constFind(manifestId);
    if (it == manifests_.cend()) {
        return nullptr;
    }

    return &it.value();
}

QVector<FileTransfer> TransferStore::transfers() const
{
    return transfers_.values().toVector();
}

QVector<FileManifest> TransferStore::manifests() const
{
    return manifests_.values().toVector();
}

void MetricsStore::addSnapshot(const MetricSnapshot &snapshot)
{
    snapshots_.append(snapshot);
}

QVector<MetricSnapshot> MetricsStore::snapshotsForPeer(const QString &peerId) const
{
    QVector<MetricSnapshot> filtered;
    filtered.reserve(snapshots_.size());

    for (const MetricSnapshot &snapshot : snapshots_) {
        if (snapshot.peerId == peerId) {
            filtered.append(snapshot);
        }
    }

    return filtered;
}

QVector<MetricSnapshot> MetricsStore::all() const
{
    return snapshots_;
}

qsizetype MetricsStore::size() const
{
    return snapshots_.size();
}

void TrustStoreModel::setTrustLevel(const QString &peerId, TrustLevel level)
{
    trustLevels_.insert(peerId, level);
}

TrustLevel TrustStoreModel::trustLevel(const QString &peerId) const
{
    return trustLevels_.value(peerId, TrustLevel::Unknown);
}

QHash<QString, TrustLevel> TrustStoreModel::all() const
{
    return trustLevels_;
}

qsizetype TrustStoreModel::size() const
{
    return trustLevels_.size();
}
