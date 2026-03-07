#include "network_services.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkDatagram>
#include <QtGlobal>

#include "identity_service.hpp"

namespace
{
QString packetTypeToString(DiscoveryPacketType type)
{
    switch (type) {
    case DiscoveryPacketType::Announce:
        return QStringLiteral("ANNOUNCE");
    case DiscoveryPacketType::Bye:
        return QStringLiteral("BYE");
    }

    return QStringLiteral("ANNOUNCE");
}

DiscoveryPacketType stringToPacketType(const QString &value)
{
    if (value == QStringLiteral("BYE")) {
        return DiscoveryPacketType::Bye;
    }

    return DiscoveryPacketType::Announce;
}

QByteArray toCompactJson(const QJsonObject &object)
{
    return QJsonDocument(object).toJson(QJsonDocument::Compact);
}

QJsonObject toJsonObject(const QByteArray &payload)
{
    return QJsonDocument::fromJson(payload).object();
}
}

DiscoveryService::DiscoveryService()
{
    QObject::connect(&socket_, &QUdpSocket::readyRead, [&]() {
        while (socket_.hasPendingDatagrams()) {
            const QNetworkDatagram datagram = socket_.receiveDatagram();
            handleDatagram(datagram.data(), datagram.senderAddress());
        }
    });

    QObject::connect(&announceTimer_, &QTimer::timeout, [&]() {
        announceNow();
    });
    QObject::connect(&ttlTimer_, &QTimer::timeout, [&]() {
        refreshPeerStatuses();
    });
}

void DiscoveryService::configure(const DiscoverySettings &settings,
                                 const IdentityService *identityService,
                                 std::function<void(const PeerDescriptor &)> onPeerUpdated,
                                 std::function<void(const QString &)> onLogMessage)
{
    settings_ = settings;
    identityService_ = identityService;
    onPeerUpdated_ = std::move(onPeerUpdated);
    onLogMessage_ = std::move(onLogMessage);
}

void DiscoveryService::seedPeers(const QVector<PeerDescriptor> &peers)
{
    for (const PeerDescriptor &peer : peers) {
        knownPeers_.insert(peer.peerId, peer);
    }
}

bool DiscoveryService::start(quint16 port)
{
    if (!socket_.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        lastError_ = socket_.errorString();
        return false;
    }

    socket_.joinMulticastGroup(QHostAddress(QStringLiteral("239.255.43.21")));
    announceTimer_.start(settings_.announceIntervalMs);
    ttlTimer_.start(1000);
    running_ = true;
    lastError_.clear();
    announceNow();
    return true;
}

void DiscoveryService::stop()
{
    if (running_) {
        const QByteArray byePacket = buildPacket(DiscoveryPacketType::Bye);
        socket_.writeDatagram(byePacket, QHostAddress::Broadcast, settings_.discoveryPort);
        socket_.writeDatagram(byePacket, QHostAddress(QStringLiteral("239.255.43.21")), settings_.discoveryPort);
    }

    announceTimer_.stop();
    ttlTimer_.stop();
    socket_.close();
    running_ = false;
}

QString DiscoveryService::lastError() const
{
    return lastError_;
}

void DiscoveryService::announceNow()
{
    if (!running_) {
        return;
    }

    const QByteArray announcePacket = buildPacket(DiscoveryPacketType::Announce);
    socket_.writeDatagram(announcePacket, QHostAddress::Broadcast, settings_.discoveryPort);
    socket_.writeDatagram(announcePacket, QHostAddress(QStringLiteral("239.255.43.21")), settings_.discoveryPort);
}

void DiscoveryService::refreshPeerStatuses()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    for (auto it = knownPeers_.begin(); it != knownPeers_.end(); ++it) {
        if (!it->lastSeenAt.isValid()) {
            continue;
        }

        const qint64 ageMs = it->lastSeenAt.msecsTo(now);
        PeerStatus newStatus = PeerStatus::Online;
        if (ageMs >= settings_.offlinePeerMs) {
            newStatus = PeerStatus::Offline;
        } else if (ageMs >= settings_.stalePeerMs) {
            newStatus = PeerStatus::Stale;
        }

        if (newStatus != it->status) {
            it->status = newStatus;
            if (onPeerUpdated_) {
                onPeerUpdated_(it.value());
            }
        }
    }
}

bool DiscoveryService::handleDatagram(const QByteArray &payload, const QHostAddress &senderAddress)
{
    return processPacket(toJsonObject(payload), senderAddress);
}

QByteArray DiscoveryService::buildPacket(DiscoveryPacketType type, std::optional<QDateTime> timestamp) const
{
    const QDateTime packetTimestamp = timestamp.value_or(QDateTime::currentDateTimeUtc());
    QJsonObject packet = packetTemplate(type, packetTimestamp);
    const QByteArray signature = identityService_ != nullptr
                                         ? identityService_->sign(signablePayload(packet)).toBase64()
                                         : QByteArray{};
    packet.insert(QStringLiteral("signature"), QString::fromUtf8(signature));
    return toCompactJson(packet);
}

QByteArray DiscoveryService::signablePayload(const QJsonObject &packet) const
{
    QJsonObject signable = packet;
    signable.remove(QStringLiteral("signature"));
    return toCompactJson(signable);
}

bool DiscoveryService::processPacket(const QJsonObject &packet, const QHostAddress &senderAddress)
{
    const QString nodeId = packet.value(QStringLiteral("nodeId")).toString();
    if (nodeId.isEmpty() || nodeId == settings_.nodeId) {
        return false;
    }

    const QDateTime timestamp = QDateTime::fromString(packet.value(QStringLiteral("timestamp")).toString(), Qt::ISODateWithMs);
    if (!timestamp.isValid()) {
        return false;
    }

    const qint64 ageMs = qAbs(timestamp.msecsTo(QDateTime::currentDateTimeUtc()));
    if (ageMs > settings_.offlinePeerMs) {
        return false;
    }

    const QByteArray publicKey = QByteArray::fromBase64(packet.value(QStringLiteral("publicKey")).toString().toUtf8());
    const QByteArray signature = QByteArray::fromBase64(packet.value(QStringLiteral("signature")).toString().toUtf8());
    if (publicKey.isEmpty() || signature.isEmpty() || identityService_ == nullptr) {
        return false;
    }

    if (!identityService_->verify(signablePayload(packet), signature, publicKey)) {
        return false;
    }

    PeerDescriptor peer = descriptorFromPacket(packet, senderAddress);
    if (stringToPacketType(packet.value(QStringLiteral("type")).toString()) == DiscoveryPacketType::Bye) {
        peer.status = PeerStatus::Offline;
    }

    knownPeers_.insert(peer.peerId, peer);
    if (onPeerUpdated_) {
        onPeerUpdated_(peer);
    }
    if (onLogMessage_) {
        onLogMessage_(QStringLiteral("Discovery packet accepted from %1").arg(peer.peerId));
    }

    return true;
}

QJsonObject DiscoveryService::packetTemplate(DiscoveryPacketType type, const QDateTime &timestamp) const
{
    QJsonArray capabilities;
    for (const QString &capability : settings_.capabilities) {
        capabilities.append(capability);
    }

    return QJsonObject{
            {QStringLiteral("type"), packetTypeToString(type)},
            {QStringLiteral("nodeId"), settings_.nodeId},
            {QStringLiteral("displayName"), settings_.displayName},
            {QStringLiteral("publicKey"), QString::fromUtf8(settings_.publicKey.toBase64())},
            {QStringLiteral("capabilities"), capabilities},
            {QStringLiteral("discoveryPort"), static_cast<int>(settings_.discoveryPort)},
            {QStringLiteral("controlPort"), static_cast<int>(settings_.controlPort)},
            {QStringLiteral("filePort"), static_cast<int>(settings_.filePort)},
            {QStringLiteral("voicePort"), static_cast<int>(settings_.voicePort)},
            {QStringLiteral("timestamp"), timestamp.toUTC().toString(Qt::ISODateWithMs)},
    };
}

PeerDescriptor DiscoveryService::descriptorFromPacket(const QJsonObject &packet, const QHostAddress &senderAddress) const
{
    QStringList capabilities;
    for (const QJsonValue &value : packet.value(QStringLiteral("capabilities")).toArray()) {
        capabilities.append(value.toString());
    }

    return PeerDescriptor{
            .peerId = packet.value(QStringLiteral("nodeId")).toString(),
            .displayName = packet.value(QStringLiteral("displayName")).toString(),
            .addresses = {senderToString(senderAddress)},
            .capabilities = capabilities,
            .discoveryPort = static_cast<quint16>(packet.value(QStringLiteral("discoveryPort")).toInt()),
            .controlPort = static_cast<quint16>(packet.value(QStringLiteral("controlPort")).toInt()),
            .filePort = static_cast<quint16>(packet.value(QStringLiteral("filePort")).toInt()),
            .voicePort = static_cast<quint16>(packet.value(QStringLiteral("voicePort")).toInt()),
            .lastSeenAt = QDateTime::currentDateTimeUtc(),
            .status = PeerStatus::Online,
            .trustLevel = TrustLevel::Unknown,
    };
}

QString DiscoveryService::senderToString(const QHostAddress &senderAddress) const
{
    if (senderAddress.protocol() == QAbstractSocket::IPv6Protocol && senderAddress.isInSubnet(QHostAddress::LocalHostIPv6, 128)) {
        return QStringLiteral("127.0.0.1");
    }

    return senderAddress.toString();
}

bool ControlServer::start(quint16 port)
{
    if (!server_.listen(QHostAddress::Any, port)) {
        lastError_ = server_.errorString();
        return false;
    }

    lastError_.clear();
    return true;
}

void ControlServer::stop()
{
    server_.close();
}

QString ControlServer::lastError() const
{
    return lastError_;
}

bool FileChannelServer::start(quint16 port)
{
    if (!server_.listen(QHostAddress::Any, port)) {
        lastError_ = server_.errorString();
        return false;
    }

    lastError_.clear();
    return true;
}

void FileChannelServer::stop()
{
    server_.close();
}

QString FileChannelServer::lastError() const
{
    return lastError_;
}

bool VoiceUdpTransport::start(quint16 port)
{
    if (!socket_.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        lastError_ = socket_.errorString();
        return false;
    }

    lastError_.clear();
    return true;
}

void VoiceUdpTransport::stop()
{
    socket_.close();
}

QString VoiceUdpTransport::lastError() const
{
    return lastError_;
}
