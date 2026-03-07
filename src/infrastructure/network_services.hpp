#pragma once

#include <functional>
#include <optional>

#include <QByteArray>
#include <QDateTime>
#include <QHostAddress>
#include <QString>
#include <QStringList>
#include <QTcpServer>
#include <QTimer>
#include <QUdpSocket>

#include "../model/domain_entities.hpp"

class IdentityService;

/**
 * @brief Настройки LAN discovery для текущего узла.
 */
struct DiscoverySettings
{
    QString nodeId;
    QString displayName;
    QByteArray publicKey;
    QStringList capabilities;
    quint16 discoveryPort = 0;
    quint16 controlPort = 0;
    quint16 filePort = 0;
    quint16 voicePort = 0;
    int announceIntervalMs = 2000;
    int stalePeerMs = 6000;
    int offlinePeerMs = 15000;
};

/**
 * @brief Тип discovery-пакета.
 */
enum class DiscoveryPacketType
{
    Announce,
    Bye,
    Probe
};

/**
 * @brief Сервис UDP discovery с announce/listen, проверкой подписи и TTL-статусами.
 */
class DiscoveryService
{
public:
    DiscoveryService();

    /**
     * @brief Конфигурирует локальный discovery state и обработчики обновления peers.
     */
    void configure(const DiscoverySettings &settings,
                   const IdentityService *identityService,
                   std::function<void(const PeerDescriptor &)> onPeerUpdated,
                   std::function<void(const QString &)> onLogMessage);

    /** @brief Предзагружает ранее известные peers для TTL-обновлений. */
    void seedPeers(const QVector<PeerDescriptor> &peers);
    /** @brief Запускает приём discovery пакетов и периодический announce. */
    bool start(quint16 port);
    /** @brief Останавливает discovery и отправляет BYE, если сервис был активен. */
    void stop();
    [[nodiscard]] QString lastError() const;

    /** @brief Принудительно рассылает announce текущего узла. */
    void announceNow();
    /** @brief Отправляет unicast probe на указанный адрес для ручного добавления peer. */
    void probeAddress(const QHostAddress &address);
    /** @brief Обновляет статусы peers по TTL. */
    void refreshPeerStatuses();
    /** @brief Обрабатывает discovery-пакет без реального сокета, удобно для тестов. */
    bool handleDatagram(const QByteArray &payload, const QHostAddress &senderAddress);
    /** @brief Собирает подписанный discovery-пакет текущего узла. */
    [[nodiscard]] QByteArray buildPacket(DiscoveryPacketType type,
                                         std::optional<QDateTime> timestamp = std::nullopt) const;

private:
    [[nodiscard]] QByteArray signablePayload(const QJsonObject &packet) const;
    [[nodiscard]] bool processPacket(const QJsonObject &packet, const QHostAddress &senderAddress);
    [[nodiscard]] QJsonObject packetTemplate(DiscoveryPacketType type, const QDateTime &timestamp) const;
    [[nodiscard]] PeerDescriptor descriptorFromPacket(const QJsonObject &packet, const QHostAddress &senderAddress) const;
    [[nodiscard]] QString senderToString(const QHostAddress &senderAddress) const;
    void sendPacketToDiscoveryTargets(const QByteArray &packet);
    void sendPacketToAddress(const QByteArray &packet, const QHostAddress &address, quint16 port);

    QUdpSocket socket_{};
    QTimer announceTimer_{};
    QTimer ttlTimer_{};
    QString lastError_{};
    DiscoverySettings settings_{};
    const IdentityService *identityService_ = nullptr;
    std::function<void(const PeerDescriptor &)> onPeerUpdated_{};
    std::function<void(const QString &)> onLogMessage_{};
    QHash<QString, PeerDescriptor> knownPeers_{};
    bool running_ = false;
};

/**
 * @brief Заглушка TCP control-plane сервера.
 */
class ControlServer
{
public:
    bool start(quint16 port);
    void stop();
    [[nodiscard]] QString lastError() const;

private:
    QTcpServer server_{};
    QString lastError_{};
};

/**
 * @brief Заглушка TCP file channel сервера.
 */
class FileChannelServer
{
public:
    bool start(quint16 port);
    void stop();
    [[nodiscard]] QString lastError() const;

private:
    QTcpServer server_{};
    QString lastError_{};
};

/**
 * @brief Заглушка UDP транспорта для голосового канала.
 */
class VoiceUdpTransport
{
public:
    bool start(quint16 port);
    void stop();
    [[nodiscard]] QString lastError() const;

private:
    QUdpSocket socket_{};
    QString lastError_{};
};
