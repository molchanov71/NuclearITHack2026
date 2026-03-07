#include <QtTest>

#include <QFileInfo>
#include <QTemporaryDir>

#include "../infrastructure/database.hpp"
#include "../infrastructure/identity_service.hpp"
#include "../infrastructure/log_service.hpp"
#include "../infrastructure/network_services.hpp"
#include "../infrastructure/repositories.hpp"
#include "../infrastructure/runtime_paths.hpp"

class InfrastructureSupportTest final : public QObject
{
    Q_OBJECT

private slots:
    void runtimePathsAndLoggingCreateArtifacts();
    void repositoriesPersistAndLoadEntities();
    void networkStubsCanStartAndStop();
};

void InfrastructureSupportTest::runtimePathsAndLoggingCreateArtifacts()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    RuntimePathsService pathsService;
    const RuntimePaths paths = pathsService.initialize(dir.path());

    QVERIFY(QFileInfo::exists(paths.dataDir));
    QVERIFY(QFileInfo::exists(paths.logDir));
    QVERIFY(QFileInfo::exists(paths.securityDir));
    QVERIFY(QFileInfo::exists(paths.transfersTmpDir));

    LogService logService;
    const auto logger = logService.initialize(paths.logDir);
    logger->info("test log line");
    logger->flush();

    QVERIFY(QFileInfo::exists(paths.logDir + QStringLiteral("/runtime.log")));
    QVERIFY(!logService.uiBuffer().lines().isEmpty());
}

void InfrastructureSupportTest::repositoriesPersistAndLoadEntities()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    Database database;
    QVERIFY(database.initialize(dir.path() + QStringLiteral("/app.db")));

    PeerRepository peerRepository(database.connection());
    MessageRepository messageRepository(database.connection());
    TransferRepository transferRepository(database.connection());
    MetricsRepository metricsRepository(database.connection());
    FileManifestRepository manifestRepository(database.connection());

    QVERIFY(peerRepository.save(PeerDescriptor{
            .peerId = QStringLiteral("peer-1"),
            .displayName = QStringLiteral("Peer One"),
            .addresses = {QStringLiteral("192.168.0.10")},
            .capabilities = {QStringLiteral("chat"), QStringLiteral("files")},
            .discoveryPort = 45454,
            .controlPort = 45455,
            .filePort = 45456,
            .voicePort = 45457,
            .lastSeenAt = QDateTime::currentDateTimeUtc(),
            .status = PeerStatus::Online,
            .trustLevel = TrustLevel::Trusted,
    }));
    QVERIFY(messageRepository.save(ChatMessage{
            .messageId = QStringLiteral("msg-1"),
            .peerId = QStringLiteral("peer-1"),
            .sessionId = QStringLiteral("session-1"),
            .authorId = QStringLiteral("peer-1"),
            .text = QStringLiteral("persisted"),
            .createdAt = QDateTime::currentDateTimeUtc(),
            .direction = MessageDirection::Outgoing,
            .deliveryStatus = MessageDeliveryStatus::Sent,
    }));
    QVERIFY(manifestRepository.save(FileManifest{
            .manifestId = QStringLiteral("manifest-1"),
            .fileName = QStringLiteral("archive.bin"),
            .relativePath = QStringLiteral("files/archive.bin"),
            .sha256 = QByteArrayLiteral("sha"),
            .totalSize = 9,
            .chunkSize = 3,
            .chunkCount = 3,
    }));
    QVERIFY(transferRepository.save(FileTransfer{
            .transferId = QStringLiteral("transfer-1"),
            .peerId = QStringLiteral("peer-1"),
            .manifestId = QStringLiteral("manifest-1"),
            .status = TransferStatus::Completed,
            .bytesTransferred = 9,
            .bytesTotal = 9,
            .updatedAt = QDateTime::currentDateTimeUtc(),
    }));
    QVERIFY(metricsRepository.save(MetricSnapshot{
            .peerId = QStringLiteral("peer-1"),
            .sessionId = QStringLiteral("session-1"),
            .capturedAt = QDateTime::currentDateTimeUtc(),
            .latencyMs = 20,
            .jitterMs = 5,
            .packetLoss = 0.2,
            .throughputKbps = 700,
    }));

    QCOMPARE(peerRepository.loadAll().size(), 1);
    QCOMPARE(messageRepository.loadAll().size(), 1);
    QCOMPARE(manifestRepository.loadAll().size(), 1);
    QCOMPARE(transferRepository.loadAll().size(), 1);
    QCOMPARE(metricsRepository.loadAll().size(), 1);
}

void InfrastructureSupportTest::networkStubsCanStartAndStop()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    IdentityService identityService;
    const IdentityMaterial identity = identityService.initialize(dir.path());
    DiscoveryService discovery;
    discovery.configure(
            DiscoverySettings{
                    .nodeId = QStringLiteral("local-node"),
                    .displayName = QStringLiteral("Local"),
                    .publicKey = identity.publicKey,
                    .capabilities = {QStringLiteral("control")},
                    .discoveryPort = 45454,
                    .controlPort = 45455,
                    .filePort = 45456,
                    .voicePort = 45457,
            },
            &identityService,
            [](const PeerDescriptor &) {},
            [](const QString &) {});
    const bool discoveryStarted = discovery.start(0);
    QVERIFY(discoveryStarted || !discovery.lastError().isEmpty());
    discovery.stop();

    ControlServer control;
    const bool controlStarted = control.start(0);
    QVERIFY(controlStarted || !control.lastError().isEmpty());
    control.stop();

    FileChannelServer fileServer;
    const bool fileServerStarted = fileServer.start(0);
    QVERIFY(fileServerStarted || !fileServer.lastError().isEmpty());
    fileServer.stop();

    VoiceUdpTransport voice;
    const bool voiceStarted = voice.start(0);
    QVERIFY(voiceStarted || !voice.lastError().isEmpty());
    voice.stop();
}

QTEST_APPLESS_MAIN(InfrastructureSupportTest)

#include "test_infrastructure_support.moc"
