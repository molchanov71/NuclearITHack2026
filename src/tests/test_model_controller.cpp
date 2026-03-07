#include <QtTest>

#include "../app/app_config.hpp"
#include "../controller/controllers.hpp"
#include "../model/stores.hpp"

class ModelControllerTest final : public QObject
{
    Q_OBJECT

private slots:
    void storesSupportCrudOperations();
    void controllersRenderExpectedLines();
    void peersTableModelRendersColumnsAndStatuses();
};

void ModelControllerTest::storesSupportCrudOperations()
{
    NodeIdentityStore identityStore;
    QVERIFY(!identityStore.hasIdentity());

    identityStore.setIdentity(NodeIdentity{
            .nodeId = QStringLiteral("node-1"),
            .displayName = QStringLiteral("Alpha"),
            .deviceName = QStringLiteral("desktop"),
            .publicKey = QByteArrayLiteral("pub"),
            .createdAt = QDateTime::currentDateTimeUtc(),
    });
    QVERIFY(identityStore.hasIdentity());
    QCOMPARE(identityStore.identity().nodeId, QStringLiteral("node-1"));

    PeerRegistryModel peerRegistry;
    peerRegistry.upsert(PeerDescriptor{
            .peerId = QStringLiteral("peer-1"),
            .displayName = QStringLiteral("Peer One"),
            .addresses = {QStringLiteral("10.0.0.1")},
            .capabilities = {QStringLiteral("chat")},
            .discoveryPort = 45454,
            .controlPort = 45455,
            .filePort = 45456,
            .voicePort = 45457,
            .lastSeenAt = QDateTime::currentDateTimeUtc(),
            .status = PeerStatus::Online,
            .trustLevel = TrustLevel::Trusted,
    });
    QVERIFY(peerRegistry.contains(QStringLiteral("peer-1")));
    QCOMPARE(peerRegistry.size(), 1);
    QVERIFY(peerRegistry.remove(QStringLiteral("peer-1")));
    QCOMPARE(peerRegistry.size(), 0);

    SessionStore sessionStore;
    sessionStore.upsert(Session{
            .sessionId = QStringLiteral("session-1"),
            .peerId = QStringLiteral("peer-1"),
            .status = SessionStatus::Connected,
            .connectedAt = QDateTime::currentDateTimeUtc(),
            .lastActivityAt = QDateTime::currentDateTimeUtc(),
            .encrypted = true,
    });
    QVERIFY(sessionStore.find(QStringLiteral("session-1")) != nullptr);
    QVERIFY(sessionStore.remove(QStringLiteral("session-1")));

    MessageStore messageStore;
    messageStore.append(ChatMessage{
            .messageId = QStringLiteral("msg-1"),
            .peerId = QStringLiteral("peer-1"),
            .sessionId = QStringLiteral("session-1"),
            .authorId = QStringLiteral("peer-1"),
            .text = QStringLiteral("hello"),
            .createdAt = QDateTime::currentDateTimeUtc(),
            .direction = MessageDirection::Incoming,
            .deliveryStatus = MessageDeliveryStatus::Delivered,
    });
    QCOMPARE(messageStore.messagesForPeer(QStringLiteral("peer-1")).size(), 1);

    TransferStore transferStore;
    transferStore.upsertManifest(FileManifest{
            .manifestId = QStringLiteral("manifest-1"),
            .fileName = QStringLiteral("archive.bin"),
            .relativePath = QStringLiteral("files/archive.bin"),
            .sha256 = QByteArrayLiteral("sha"),
            .totalSize = 42,
            .chunkSize = 7,
            .chunkCount = 6,
    });
    transferStore.upsertTransfer(FileTransfer{
            .transferId = QStringLiteral("transfer-1"),
            .peerId = QStringLiteral("peer-1"),
            .manifestId = QStringLiteral("manifest-1"),
            .status = TransferStatus::Transferring,
            .bytesTransferred = 21,
            .bytesTotal = 42,
            .updatedAt = QDateTime::currentDateTimeUtc(),
    });
    QVERIFY(transferStore.findManifest(QStringLiteral("manifest-1")) != nullptr);
    QVERIFY(transferStore.findTransfer(QStringLiteral("transfer-1")) != nullptr);
    QVERIFY(transferStore.removeTransfer(QStringLiteral("transfer-1")));

    MetricsStore metricsStore;
    metricsStore.addSnapshot(MetricSnapshot{
            .peerId = QStringLiteral("peer-1"),
            .sessionId = QStringLiteral("session-1"),
            .capturedAt = QDateTime::currentDateTimeUtc(),
            .latencyMs = 15,
            .jitterMs = 3,
            .packetLoss = 0.5,
            .throughputKbps = 1000,
    });
    QCOMPARE(metricsStore.snapshotsForPeer(QStringLiteral("peer-1")).size(), 1);

    TrustStoreModel trustStore;
    QCOMPARE(trustStore.trustLevel(QStringLiteral("peer-1")), TrustLevel::Unknown);
    trustStore.setTrustLevel(QStringLiteral("peer-1"), TrustLevel::Blocked);
    QCOMPARE(trustStore.trustLevel(QStringLiteral("peer-1")), TrustLevel::Blocked);
}

void ModelControllerTest::controllersRenderExpectedLines()
{
    AppConfig config = AppConfig::loadDefault();
    config.appName = QStringLiteral("TestApp");
    config.dataDir = QStringLiteral("/tmp/test-app");

    PeerRegistryModel peerRegistry;
    PeersTableModel peersTableModel;
    peerRegistry.upsert(PeerDescriptor{
            .peerId = QStringLiteral("peer-1"),
            .displayName = QStringLiteral("Peer One"),
            .addresses = {QStringLiteral("127.0.0.1")},
            .capabilities = {QStringLiteral("chat"), QStringLiteral("control")},
            .discoveryPort = 45454,
            .controlPort = 45455,
            .filePort = 45456,
            .voicePort = 45457,
            .lastSeenAt = QDateTime::currentDateTimeUtc(),
            .status = PeerStatus::Online,
            .trustLevel = TrustLevel::Trusted,
    });

    SessionStore sessionStore;
    sessionStore.upsert(Session{
            .sessionId = QStringLiteral("session-1"),
            .peerId = QStringLiteral("peer-1"),
            .status = SessionStatus::Connected,
            .connectedAt = QDateTime::currentDateTimeUtc(),
            .lastActivityAt = QDateTime::currentDateTimeUtc(),
            .encrypted = true,
    });

    MessageStore messageStore;
    messageStore.append(ChatMessage{
            .messageId = QStringLiteral("msg-1"),
            .peerId = QStringLiteral("peer-1"),
            .sessionId = QStringLiteral("session-1"),
            .authorId = QStringLiteral("peer-1"),
            .text = QStringLiteral("hello"),
            .createdAt = QDateTime::currentDateTimeUtc(),
            .direction = MessageDirection::Incoming,
            .deliveryStatus = MessageDeliveryStatus::Delivered,
    });

    TransferStore transferStore;
    transferStore.upsertTransfer(FileTransfer{
            .transferId = QStringLiteral("transfer-1"),
            .peerId = QStringLiteral("peer-1"),
            .manifestId = QStringLiteral("manifest-1"),
            .status = TransferStatus::Pending,
            .bytesTransferred = 1,
            .bytesTotal = 2,
            .updatedAt = QDateTime::currentDateTimeUtc(),
    });

    MetricsStore metricsStore;
    metricsStore.addSnapshot(MetricSnapshot{
            .peerId = QStringLiteral("peer-1"),
            .sessionId = QStringLiteral("session-1"),
            .capturedAt = QDateTime::currentDateTimeUtc(),
            .latencyMs = 10,
            .jitterMs = 1,
            .packetLoss = 0.0,
            .throughputKbps = 256,
    });

    NodeIdentityStore identityStore;
    identityStore.setIdentity(NodeIdentity{
            .nodeId = QStringLiteral("node-1"),
            .displayName = QStringLiteral("Alpha"),
            .deviceName = QStringLiteral("desktop"),
            .publicKey = QByteArrayLiteral("pub"),
            .createdAt = QDateTime::currentDateTimeUtc(),
    });

    TrustStoreModel trustStore;
    trustStore.setTrustLevel(QStringLiteral("peer-1"), TrustLevel::Trusted);

    AppController appController(config, metricsStore);
    int probeRequests = 0;
    PeerController peerController(peerRegistry,
                                  peersTableModel,
                                  config,
                                  {},
                                  [&probeRequests](const QString &) { ++probeRequests; });
    SessionController sessionController(sessionStore);
    ChatController chatController(messageStore);
    FileTransferController fileTransferController(transferStore);
    CallController callController(metricsStore);
    DiagnosticsController diagnosticsController(identityStore, metricsStore, trustStore);

    peerController.refresh();
    QVERIFY(appController.summaryLines().join('\n').contains(QStringLiteral("TestApp")));
    QVERIFY(peerController.peerLines().join('\n').contains(QStringLiteral("Peer One")));
    QCOMPARE(peerController.tableModel().rowCount(), 1);
    QVERIFY(sessionController.sessionLines().join('\n').contains(QStringLiteral("session-1")));
    QVERIFY(chatController.chatLines().join('\n').contains(QStringLiteral("hello")));
    QVERIFY(fileTransferController.transferLines().join('\n').contains(QStringLiteral("transfer-1")));
    QVERIFY(callController.callLines().join('\n').contains(QStringLiteral("voice pipeline")));
    QVERIFY(diagnosticsController.diagnosticLines().join('\n').contains(QStringLiteral("node-1")));

    peerController.addManualPeer(QStringLiteral("192.168.1.42"));
    QVERIFY(peerRegistry.contains(QStringLiteral("manual-192.168.1.42")));
    QCOMPARE(probeRequests, 1);
}

void ModelControllerTest::peersTableModelRendersColumnsAndStatuses()
{
    PeerRegistryModel peerRegistry;
    peerRegistry.upsert(PeerDescriptor{
            .peerId = QStringLiteral("peer-a"),
            .displayName = QStringLiteral("Alpha"),
            .addresses = {QStringLiteral("10.0.0.1")},
            .capabilities = {QStringLiteral("chat"), QStringLiteral("control")},
            .discoveryPort = 45454,
            .controlPort = 45455,
            .filePort = 45456,
            .voicePort = 45457,
            .lastSeenAt = QDateTime::currentDateTimeUtc(),
            .status = PeerStatus::Stale,
            .trustLevel = TrustLevel::Trusted,
    });

    PeersTableModel tableModel;
    tableModel.refreshFromRegistry(peerRegistry);

    QCOMPARE(tableModel.rowCount(), 1);
    QCOMPARE(tableModel.columnCount(), 5);
    QCOMPARE(tableModel.headerData(0, Qt::Horizontal, Qt::DisplayRole).toString(), QStringLiteral("Имя"));
    QCOMPARE(tableModel.data(tableModel.index(0, 0), Qt::DisplayRole).toString(), QStringLiteral("Alpha"));
    QVERIFY(tableModel.data(tableModel.index(0, 2), Qt::DisplayRole).toString().contains(QStringLiteral("chat")));
    QVERIFY(tableModel.data(tableModel.index(0, 3), Qt::DisplayRole).toString().contains(QStringLiteral("10.0.0.1")));
    QCOMPARE(tableModel.data(tableModel.index(0, 4), Qt::DisplayRole).toString(), QStringLiteral("STALE"));

    QVERIFY(peerRegistry.updateStatus(QStringLiteral("peer-a"), PeerStatus::Offline));
    tableModel.refreshFromRegistry(peerRegistry);
    QCOMPARE(tableModel.data(tableModel.index(0, 4), Qt::DisplayRole).toString(), QStringLiteral("OFFLINE"));
    QVERIFY(tableModel.peerAt(0) != nullptr);
    QVERIFY(tableModel.peerAt(1) == nullptr);
}

QTEST_APPLESS_MAIN(ModelControllerTest)

#include "test_model_controller.moc"
