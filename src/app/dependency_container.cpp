#include "dependency_container.hpp"

#include <QDateTime>
#include <QJsonObject>

#include <opus/opus.h>
#include <spdlog/spdlog.h>

void DependencyContainer::initialize(const AppConfig &config)
{
    config_ = config;
    paths_ = runtimePathsService_.initialize(config_.dataDir);

    logger_ = logService_.initialize(paths_.logDir);
    logger_->info("Runtime directories prepared at {}", paths_.dataDir.toStdString());

    database_.initialize(config_.dbPath);
    logger_->info("SQLite opened at {}", config_.dbPath.toStdString());
    peerRepository_ = std::make_unique<PeerRepository>(database_.connection());
    messageRepository_ = std::make_unique<MessageRepository>(database_.connection());
    transferRepository_ = std::make_unique<TransferRepository>(database_.connection());
    metricsRepository_ = std::make_unique<MetricsRepository>(database_.connection());
    fileManifestRepository_ = std::make_unique<FileManifestRepository>(database_.connection());

    const IdentityMaterial identityMaterial = identityService_.initialize(paths_.securityDir);
    nodeIdentityStore_.setIdentity(NodeIdentity{
            .nodeId = QString::fromUtf8(identityMaterial.nodeId),
            .displayName = config_.appName,
            .deviceName = QStringLiteral("desktop"),
            .publicKey = identityMaterial.publicKey,
            .createdAt = QDateTime::currentDateTimeUtc(),
    });

    peerRegistry_.upsert(PeerDescriptor{
            .peerId = QStringLiteral("peer-demo"),
            .displayName = QStringLiteral("Demo peer"),
            .addresses = {QStringLiteral("127.0.0.1")},
            .lastSeenAt = QDateTime::currentDateTimeUtc(),
            .status = PeerStatus::Online,
            .trustLevel = TrustLevel::Trusted,
    });
    trustStore_.setTrustLevel(QStringLiteral("peer-demo"), TrustLevel::Trusted);
    peerRepository_->save(*peerRegistry_.find(QStringLiteral("peer-demo")));
    sessionStore_.upsert(Session{
            .sessionId = QStringLiteral("session-demo"),
            .peerId = QStringLiteral("peer-demo"),
            .status = SessionStatus::Connected,
            .connectedAt = QDateTime::currentDateTimeUtc(),
            .lastActivityAt = QDateTime::currentDateTimeUtc(),
            .encrypted = true,
    });
    messageStore_.append(ChatMessage{
            .messageId = QStringLiteral("msg-demo"),
            .peerId = QStringLiteral("peer-demo"),
            .sessionId = QStringLiteral("session-demo"),
            .authorId = QStringLiteral("peer-demo"),
            .text = QStringLiteral("Фундамент runtime уже поднят."),
            .createdAt = QDateTime::currentDateTimeUtc(),
            .direction = MessageDirection::Incoming,
            .deliveryStatus = MessageDeliveryStatus::Delivered,
    });
    messageRepository_->save(messageStore_.all().constFirst());
    transferStore_.upsertManifest(FileManifest{
            .manifestId = QStringLiteral("manifest-demo"),
            .fileName = QStringLiteral("readme.txt"),
            .relativePath = QStringLiteral("demo/readme.txt"),
            .sha256 = QByteArrayLiteral("demo"),
            .totalSize = 1024,
            .chunkSize = 256,
            .chunkCount = 4,
    });
    fileManifestRepository_->save(transferStore_.manifests().constFirst());
    transferStore_.upsertTransfer(FileTransfer{
            .transferId = QStringLiteral("transfer-demo"),
            .peerId = QStringLiteral("peer-demo"),
            .manifestId = QStringLiteral("manifest-demo"),
            .status = TransferStatus::Pending,
            .bytesTransferred = 128,
            .bytesTotal = 1024,
            .updatedAt = QDateTime::currentDateTimeUtc(),
    });
    transferRepository_->save(transferStore_.transfers().constFirst());
    metricsStore_.addSnapshot(MetricSnapshot{
            .peerId = QStringLiteral("peer-demo"),
            .sessionId = QStringLiteral("session-demo"),
            .capturedAt = QDateTime::currentDateTimeUtc(),
            .latencyMs = 12,
            .jitterMs = 2,
            .packetLoss = 0.1,
            .throughputKbps = 512,
    });
    metricsRepository_->save(metricsStore_.all().constFirst());

    appController_ = std::make_unique<AppController>(config_, metricsStore_);
    peerController_ = std::make_unique<PeerController>(peerRegistry_);
    sessionController_ = std::make_unique<SessionController>(sessionStore_);
    chatController_ = std::make_unique<ChatController>(messageStore_);
    fileTransferController_ = std::make_unique<FileTransferController>(transferStore_);
    callController_ = std::make_unique<CallController>(metricsStore_);
    diagnosticsController_ = std::make_unique<DiagnosticsController>(nodeIdentityStore_, metricsStore_, trustStore_);

    const Envelope ping{
            .messageId = QStringLiteral("bootstrap-ping"),
            .sessionId = QStringLiteral("session-demo"),
            .sourceNodeId = nodeIdentityStore_.identity().nodeId,
            .targetNodeId = QStringLiteral("peer-demo"),
            .type = ControlMessageType::Ping,
            .payload = QJsonObject{{QStringLiteral("probe"), QStringLiteral("runtime")}},
    };
    ackTracker_.registerMessage(ping.messageId);
    logger_->info("Protocol bootstrap frame size={} bytes", messageCodec_.encode(ping).size());
    logger_->info("libsodium initialized, Opus runtime={}", opus_get_version_string());

    if (config_.enableNetwork) {
        if (config_.enableDiscovery && !discoveryService_.start(config_.discoveryPort)) {
            logger_->warn("Discovery bind failed: {}", discoveryService_.lastError().toStdString());
        }
        if (!controlServer_.start(config_.controlPort)) {
            logger_->warn("Control server failed: {}", controlServer_.lastError().toStdString());
        }
        if (!fileChannelServer_.start(config_.filePort)) {
            logger_->warn("File channel failed: {}", fileChannelServer_.lastError().toStdString());
        }
        if (!voiceUdpTransport_.start(config_.voicePort)) {
            logger_->warn("Voice transport failed: {}", voiceUdpTransport_.lastError().toStdString());
        }
    }

    logger_->info("Runtime initialized");
    logger_->info("Model layer ready: peers={}, sessions={}, messages={}, transfers={}",
                  peerRegistry_.size(),
                  sessionStore_.size(),
                  messageStore_.size(),
                  transferStore_.transfers().size());

    initialized_ = true;
}

void DependencyContainer::shutdown()
{
    if (logger_ != nullptr) {
        logger_->info("Runtime shutdown");
    }

    discoveryService_.stop();
    controlServer_.stop();
    fileChannelServer_.stop();
    voiceUdpTransport_.stop();
    database_.shutdown();
    fileManifestRepository_.reset();
    metricsRepository_.reset();
    transferRepository_.reset();
    messageRepository_.reset();
    peerRepository_.reset();
    diagnosticsController_.reset();
    callController_.reset();
    fileTransferController_.reset();
    chatController_.reset();
    sessionController_.reset();
    peerController_.reset();
    appController_.reset();
    spdlog::drop("app");
    logger_.reset();
    initialized_ = false;
}

const AppConfig &DependencyContainer::config() const
{
    return config_;
}

spdlog::logger &DependencyContainer::logger() const
{
    return *logger_;
}

const UiLogBuffer &DependencyContainer::uiLogBuffer() const
{
    return logService_.uiBuffer();
}

const RuntimePaths &DependencyContainer::paths() const
{
    return paths_;
}

const Database &DependencyContainer::database() const
{
    return database_;
}

const PeerRepository &DependencyContainer::peerRepository() const
{
    return *peerRepository_;
}

const MessageRepository &DependencyContainer::messageRepository() const
{
    return *messageRepository_;
}

const TransferRepository &DependencyContainer::transferRepository() const
{
    return *transferRepository_;
}

const MetricsRepository &DependencyContainer::metricsRepository() const
{
    return *metricsRepository_;
}

const FileManifestRepository &DependencyContainer::fileManifestRepository() const
{
    return *fileManifestRepository_;
}

const IdentityService &DependencyContainer::identityService() const
{
    return identityService_;
}

const MessageCodec &DependencyContainer::messageCodec() const
{
    return messageCodec_;
}

NodeIdentityStore &DependencyContainer::nodeIdentityStore()
{
    return nodeIdentityStore_;
}

PeerRegistryModel &DependencyContainer::peerRegistry()
{
    return peerRegistry_;
}

SessionStore &DependencyContainer::sessionStore()
{
    return sessionStore_;
}

MessageStore &DependencyContainer::messageStore()
{
    return messageStore_;
}

TransferStore &DependencyContainer::transferStore()
{
    return transferStore_;
}

MetricsStore &DependencyContainer::metricsStore()
{
    return metricsStore_;
}

TrustStoreModel &DependencyContainer::trustStore()
{
    return trustStore_;
}

AppController &DependencyContainer::appController()
{
    return *appController_;
}

PeerController &DependencyContainer::peerController()
{
    return *peerController_;
}

SessionController &DependencyContainer::sessionController()
{
    return *sessionController_;
}

ChatController &DependencyContainer::chatController()
{
    return *chatController_;
}

FileTransferController &DependencyContainer::fileTransferController()
{
    return *fileTransferController_;
}

CallController &DependencyContainer::callController()
{
    return *callController_;
}

DiagnosticsController &DependencyContainer::diagnosticsController()
{
    return *diagnosticsController_;
}

bool DependencyContainer::isInitialized() const
{
    return initialized_;
}
