#include <QtTest>

#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>

#include "../infrastructure/identity_service.hpp"
#include "../infrastructure/network_services.hpp"
#include "../model/stores.hpp"

class DiscoveryTest final : public QObject
{
    Q_OBJECT

private slots:
    void validAnnounceUpdatesPeerRegistry();
    void invalidSignatureIsRejected();
    void byeMarksPeerOffline();
    void selfAndExpiredPacketsAreRejected();
    void probePacketIsAccepted();
    void repeatedAnnounceKeepsPeerOnline();
    void ttlMovesPeerToStaleAndOffline();
};

void DiscoveryTest::validAnnounceUpdatesPeerRegistry()
{
    QTemporaryDir localDir;
    QTemporaryDir remoteDir;
    QVERIFY(localDir.isValid());
    QVERIFY(remoteDir.isValid());

    IdentityService localIdentityService;
    const IdentityMaterial localIdentity = localIdentityService.initialize(localDir.path());
    IdentityService remoteIdentityService;
    const IdentityMaterial remoteIdentity = remoteIdentityService.initialize(remoteDir.path());

    PeerRegistryModel peerRegistry;
    DiscoveryService discovery;
    discovery.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("local-node"),
                    .displayName = QStringLiteral("Local"),
                    .publicKey = localIdentity.publicKey,
                    .capabilities = {QStringLiteral("control")},
                    .discoveryPort = 45454,
                    .controlPort = 45455,
                    .filePort = 45456,
                    .voicePort = 45457,
            },
            &localIdentityService,
            [&peerRegistry](const PeerDescriptor &peer) { peerRegistry.upsert(peer); },
            [](const QString &) {});

    DiscoveryService remoteEmitter;
    remoteEmitter.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("remote-node"),
                    .displayName = QStringLiteral("Remote"),
                    .publicKey = remoteIdentity.publicKey,
                    .capabilities = {QStringLiteral("chat"), QStringLiteral("files")},
                    .discoveryPort = 45454,
                    .controlPort = 50001,
                    .filePort = 50002,
                    .voicePort = 50003,
            },
            &remoteIdentityService,
            [](const PeerDescriptor &) {},
            [](const QString &) {});

    QVERIFY(discovery.handleDatagram(remoteEmitter.buildPacket(DiscoveryPacketType::Announce), QHostAddress(QStringLiteral("192.168.1.20"))));
    const PeerDescriptor *peer = peerRegistry.find(QStringLiteral("remote-node"));
    QVERIFY(peer != nullptr);
    QCOMPARE(peer->displayName, QStringLiteral("Remote"));
    QCOMPARE(peer->addresses.constFirst(), QStringLiteral("192.168.1.20"));
    QCOMPARE(peer->controlPort, 50001);
    QCOMPARE(peer->status, PeerStatus::Online);
}

void DiscoveryTest::invalidSignatureIsRejected()
{
    QTemporaryDir localDir;
    QTemporaryDir remoteDir;
    QVERIFY(localDir.isValid());
    QVERIFY(remoteDir.isValid());

    IdentityService localIdentityService;
    const IdentityMaterial localIdentity = localIdentityService.initialize(localDir.path());
    IdentityService remoteIdentityService;
    const IdentityMaterial remoteIdentity = remoteIdentityService.initialize(remoteDir.path());

    int callbackCount = 0;
    DiscoveryService discovery;
    discovery.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("local-node"),
                    .displayName = QStringLiteral("Local"),
                    .publicKey = localIdentity.publicKey,
                    .capabilities = {QStringLiteral("control")},
                    .discoveryPort = 45454,
                    .controlPort = 45455,
                    .filePort = 45456,
                    .voicePort = 45457,
            },
            &localIdentityService,
            [&callbackCount](const PeerDescriptor &) { ++callbackCount; },
            [](const QString &) {});

    DiscoveryService remoteEmitter;
    remoteEmitter.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("remote-node"),
                    .displayName = QStringLiteral("Remote"),
                    .publicKey = remoteIdentity.publicKey,
                    .capabilities = {QStringLiteral("chat")},
                    .discoveryPort = 45454,
                    .controlPort = 50001,
                    .filePort = 50002,
                    .voicePort = 50003,
            },
            &remoteIdentityService,
            [](const PeerDescriptor &) {},
            [](const QString &) {});

    QJsonObject packet = QJsonDocument::fromJson(remoteEmitter.buildPacket(DiscoveryPacketType::Announce)).object();
    packet.insert(QStringLiteral("displayName"), QStringLiteral("Tampered"));
    QVERIFY(!discovery.handleDatagram(QJsonDocument(packet).toJson(QJsonDocument::Compact), QHostAddress(QStringLiteral("192.168.1.21"))));
    QCOMPARE(callbackCount, 0);
}

void DiscoveryTest::byeMarksPeerOffline()
{
    QTemporaryDir localDir;
    QTemporaryDir remoteDir;
    QVERIFY(localDir.isValid());
    QVERIFY(remoteDir.isValid());

    IdentityService localIdentityService;
    const IdentityMaterial localIdentity = localIdentityService.initialize(localDir.path());
    IdentityService remoteIdentityService;
    const IdentityMaterial remoteIdentity = remoteIdentityService.initialize(remoteDir.path());

    PeerRegistryModel peerRegistry;
    DiscoveryService discovery;
    discovery.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("local-node"),
                    .displayName = QStringLiteral("Local"),
                    .publicKey = localIdentity.publicKey,
                    .capabilities = {QStringLiteral("control")},
                    .discoveryPort = 45454,
                    .controlPort = 45455,
                    .filePort = 45456,
                    .voicePort = 45457,
            },
            &localIdentityService,
            [&peerRegistry](const PeerDescriptor &peer) { peerRegistry.upsert(peer); },
            [](const QString &) {});

    DiscoveryService remoteEmitter;
    remoteEmitter.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("remote-node"),
                    .displayName = QStringLiteral("Remote"),
                    .publicKey = remoteIdentity.publicKey,
                    .capabilities = {QStringLiteral("chat")},
                    .discoveryPort = 45454,
                    .controlPort = 50001,
                    .filePort = 50002,
                    .voicePort = 50003,
            },
            &remoteIdentityService,
            [](const PeerDescriptor &) {},
            [](const QString &) {});

    QVERIFY(discovery.handleDatagram(remoteEmitter.buildPacket(DiscoveryPacketType::Announce), QHostAddress::LocalHost));
    QVERIFY(discovery.handleDatagram(remoteEmitter.buildPacket(DiscoveryPacketType::Bye), QHostAddress::LocalHost));
    const PeerDescriptor *peer = peerRegistry.find(QStringLiteral("remote-node"));
    QVERIFY(peer != nullptr);
    QCOMPARE(peer->status, PeerStatus::Offline);
}

void DiscoveryTest::selfAndExpiredPacketsAreRejected()
{
    QTemporaryDir localDir;
    QVERIFY(localDir.isValid());

    IdentityService localIdentityService;
    const IdentityMaterial localIdentity = localIdentityService.initialize(localDir.path());

    int callbackCount = 0;
    DiscoveryService discovery;
    discovery.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("local-node"),
                    .displayName = QStringLiteral("Local"),
                    .publicKey = localIdentity.publicKey,
                    .capabilities = {QStringLiteral("control")},
                    .discoveryPort = 45454,
                    .controlPort = 45455,
                    .filePort = 45456,
                    .voicePort = 45457,
                    .offlinePeerMs = 5000,
            },
            &localIdentityService,
            [&callbackCount](const PeerDescriptor &) { ++callbackCount; },
            [](const QString &) {});

    DiscoveryService localEmitter;
    localEmitter.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("local-node"),
                    .displayName = QStringLiteral("Local"),
                    .publicKey = localIdentity.publicKey,
                    .capabilities = {QStringLiteral("control")},
                    .discoveryPort = 45454,
                    .controlPort = 45455,
                    .filePort = 45456,
                    .voicePort = 45457,
                    .offlinePeerMs = 5000,
            },
            &localIdentityService,
            [](const PeerDescriptor &) {},
            [](const QString &) {});

    QVERIFY(!discovery.handleDatagram(localEmitter.buildPacket(DiscoveryPacketType::Announce), QHostAddress::LocalHost));
    QVERIFY(!discovery.handleDatagram(
            localEmitter.buildPacket(DiscoveryPacketType::Announce, QDateTime::currentDateTimeUtc().addSecs(-60)),
            QHostAddress(QStringLiteral("10.0.0.2"))));
    QCOMPARE(callbackCount, 0);
}

void DiscoveryTest::probePacketIsAccepted()
{
    QTemporaryDir localDir;
    QTemporaryDir remoteDir;
    QVERIFY(localDir.isValid());
    QVERIFY(remoteDir.isValid());

    IdentityService localIdentityService;
    const IdentityMaterial localIdentity = localIdentityService.initialize(localDir.path());
    IdentityService remoteIdentityService;
    const IdentityMaterial remoteIdentity = remoteIdentityService.initialize(remoteDir.path());

    PeerRegistryModel peerRegistry;
    DiscoveryService discovery;
    discovery.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("local-node"),
                    .displayName = QStringLiteral("Local"),
                    .publicKey = localIdentity.publicKey,
                    .capabilities = {QStringLiteral("control")},
                    .discoveryPort = 45454,
                    .controlPort = 45455,
                    .filePort = 45456,
                    .voicePort = 45457,
            },
            &localIdentityService,
            [&peerRegistry](const PeerDescriptor &peer) { peerRegistry.upsert(peer); },
            [](const QString &) {});

    DiscoveryService remoteEmitter;
    remoteEmitter.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("remote-node"),
                    .displayName = QStringLiteral("Remote"),
                    .publicKey = remoteIdentity.publicKey,
                    .capabilities = {QStringLiteral("chat")},
                    .discoveryPort = 45454,
                    .controlPort = 50001,
                    .filePort = 50002,
                    .voicePort = 50003,
            },
            &remoteIdentityService,
            [](const PeerDescriptor &) {},
            [](const QString &) {});

    QVERIFY(discovery.handleDatagram(remoteEmitter.buildPacket(DiscoveryPacketType::Probe), QHostAddress(QStringLiteral("192.168.1.30"))));
    const PeerDescriptor *peer = peerRegistry.find(QStringLiteral("remote-node"));
    QVERIFY(peer != nullptr);
    QCOMPARE(peer->status, PeerStatus::Online);
}

void DiscoveryTest::repeatedAnnounceKeepsPeerOnline()
{
    QTemporaryDir localDir;
    QTemporaryDir remoteDir;
    QVERIFY(localDir.isValid());
    QVERIFY(remoteDir.isValid());

    IdentityService localIdentityService;
    const IdentityMaterial localIdentity = localIdentityService.initialize(localDir.path());
    IdentityService remoteIdentityService;
    const IdentityMaterial remoteIdentity = remoteIdentityService.initialize(remoteDir.path());

    PeerRegistryModel peerRegistry;
    DiscoveryService discovery;
    discovery.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("local-node"),
                    .displayName = QStringLiteral("Local"),
                    .publicKey = localIdentity.publicKey,
                    .capabilities = {QStringLiteral("control")},
                    .discoveryPort = 45454,
                    .controlPort = 45455,
                    .filePort = 45456,
                    .voicePort = 45457,
                    .stalePeerMs = 1000,
                    .offlinePeerMs = 3000,
            },
            &localIdentityService,
            [&peerRegistry](const PeerDescriptor &peer) { peerRegistry.upsert(peer); },
            [](const QString &) {});

    DiscoveryService remoteEmitter;
    remoteEmitter.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("remote-node"),
                    .displayName = QStringLiteral("Remote"),
                    .publicKey = remoteIdentity.publicKey,
                    .capabilities = {QStringLiteral("chat")},
                    .discoveryPort = 45454,
                    .controlPort = 50001,
                    .filePort = 50002,
                    .voicePort = 50003,
                    .stalePeerMs = 1000,
                    .offlinePeerMs = 3000,
            },
            &remoteIdentityService,
            [](const PeerDescriptor &) {},
            [](const QString &) {});

    QVERIFY(discovery.handleDatagram(remoteEmitter.buildPacket(DiscoveryPacketType::Announce), QHostAddress(QStringLiteral("192.168.1.50"))));
    QTest::qSleep(500);
    QVERIFY(discovery.handleDatagram(remoteEmitter.buildPacket(DiscoveryPacketType::Announce), QHostAddress(QStringLiteral("192.168.1.50"))));
    discovery.refreshPeerStatuses();

    const PeerDescriptor *peer = peerRegistry.find(QStringLiteral("remote-node"));
    QVERIFY(peer != nullptr);
    QCOMPARE(peer->status, PeerStatus::Online);
}

void DiscoveryTest::ttlMovesPeerToStaleAndOffline()
{
    QTemporaryDir localDir;
    QVERIFY(localDir.isValid());

    IdentityService localIdentityService;
    const IdentityMaterial localIdentity = localIdentityService.initialize(localDir.path());

    PeerRegistryModel peerRegistry;
    peerRegistry.upsert(PeerDescriptor{
            .peerId = QStringLiteral("peer-old"),
            .displayName = QStringLiteral("Old Peer"),
            .addresses = {QStringLiteral("10.0.0.1")},
            .capabilities = {QStringLiteral("chat")},
            .discoveryPort = 45454,
            .controlPort = 45455,
            .filePort = 45456,
            .voicePort = 45457,
            .lastSeenAt = QDateTime::currentDateTimeUtc().addMSecs(-20000),
            .status = PeerStatus::Online,
            .trustLevel = TrustLevel::Unknown,
    });

    DiscoveryService discovery;
    discovery.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("local-node"),
                    .displayName = QStringLiteral("Local"),
                    .publicKey = localIdentity.publicKey,
                    .capabilities = {QStringLiteral("control")},
                    .discoveryPort = 45454,
                    .controlPort = 45455,
                    .filePort = 45456,
                    .voicePort = 45457,
                    .stalePeerMs = 1000,
                    .offlinePeerMs = 5000,
            },
            &localIdentityService,
            [&peerRegistry](const PeerDescriptor &peer) { peerRegistry.upsert(peer); },
            [](const QString &) {});
    discovery.seedPeers(peerRegistry.all());
    discovery.refreshPeerStatuses();

    const PeerDescriptor *peer = peerRegistry.find(QStringLiteral("peer-old"));
    QVERIFY(peer != nullptr);
    QCOMPARE(peer->status, PeerStatus::Offline);

    peerRegistry.upsert(PeerDescriptor{
            .peerId = QStringLiteral("peer-stale"),
            .displayName = QStringLiteral("Stale Peer"),
            .addresses = {QStringLiteral("10.0.0.2")},
            .capabilities = {QStringLiteral("chat")},
            .discoveryPort = 45454,
            .controlPort = 45455,
            .filePort = 45456,
            .voicePort = 45457,
            .lastSeenAt = QDateTime::currentDateTimeUtc().addMSecs(-2000),
            .status = PeerStatus::Online,
            .trustLevel = TrustLevel::Unknown,
    });
    discovery.seedPeers(peerRegistry.all());
    discovery.refreshPeerStatuses();
    peer = peerRegistry.find(QStringLiteral("peer-stale"));
    QVERIFY(peer != nullptr);
    QCOMPARE(peer->status, PeerStatus::Stale);
}

QTEST_APPLESS_MAIN(DiscoveryTest)

#include "test_discovery.moc"
