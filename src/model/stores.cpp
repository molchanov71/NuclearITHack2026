#include "stores.hpp"

#include <algorithm>

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

bool PeerRegistryModel::updateStatus(const QString &peerId, PeerStatus status)
{
    const auto it = peers_.find(peerId);
    if (it == peers_.end()) {
        return false;
    }

    it->status = status;
    return true;
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

PeersTableModel::PeersTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int PeersTableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : peers_.size();
}

int PeersTableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 5;
}

QVariant PeersTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= peers_.size()) {
        return {};
    }

    const PeerDescriptor &peer = peers_.at(index.row());
    if (role != Qt::DisplayRole) {
        return {};
    }

    switch (index.column()) {
    case 0:
        return peer.displayName;
    case 1:
        return peer.peerId;
    case 2:
        return peer.capabilities.join(QStringLiteral(", "));
    case 3:
        return formatEndpoints(peer);
    case 4:
        return formatStatus(peer.status);
    default:
        return {};
    }
}

QVariant PeersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    switch (section) {
    case 0:
        return QStringLiteral("Имя");
    case 1:
        return QStringLiteral("Node ID");
    case 2:
        return QStringLiteral("Capabilities");
    case 3:
        return QStringLiteral("IP / Ports");
    case 4:
        return QStringLiteral("Состояние");
    default:
        return {};
    }
}

void PeersTableModel::refreshFromRegistry(const PeerRegistryModel &peerRegistry)
{
    beginResetModel();
    peers_ = peerRegistry.all();
    std::sort(peers_.begin(), peers_.end(), [](const PeerDescriptor &lhs, const PeerDescriptor &rhs) {
        return lhs.displayName.toLower() < rhs.displayName.toLower();
    });
    endResetModel();
}

const PeerDescriptor *PeersTableModel::peerAt(int row) const
{
    if (row < 0 || row >= peers_.size()) {
        return nullptr;
    }

    return &peers_.at(row);
}

QString PeersTableModel::formatStatus(PeerStatus status) const
{
    switch (status) {
    case PeerStatus::Online:
        return QStringLiteral("ONLINE");
    case PeerStatus::Stale:
        return QStringLiteral("STALE");
    case PeerStatus::Offline:
        return QStringLiteral("OFFLINE");
    }

    return QStringLiteral("UNKNOWN");
}

QString PeersTableModel::formatEndpoints(const PeerDescriptor &peer) const
{
    const QString address = peer.addresses.isEmpty() ? QStringLiteral("n/a") : peer.addresses.constFirst();
    return QStringLiteral("%1 | c:%2 f:%3 v:%4")
            .arg(address)
            .arg(peer.controlPort)
            .arg(peer.filePort)
            .arg(peer.voicePort);
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
