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

    for (const PeerDescriptor &peer : peerRepository_->loadAll()) {
        peerRegistry_.upsert(peer);
    }
    peersTableModel_.refreshFromRegistry(peerRegistry_);

    appController_ = std::make_unique<AppController>(config_, metricsStore_);
    peerController_ = std::make_unique<PeerController>(
            peerRegistry_,
            peersTableModel_,
            config_,
            [this](const PeerDescriptor &peer) { handlePeerUpdated(peer); });
    sessionController_ = std::make_unique<SessionController>(sessionStore_);
    chatController_ = std::make_unique<ChatController>(messageStore_);
    fileTransferController_ = std::make_unique<FileTransferController>(transferStore_);
    callController_ = std::make_unique<CallController>(metricsStore_);
    diagnosticsController_ = std::make_unique<DiagnosticsController>(nodeIdentityStore_, metricsStore_, trustStore_);

    const Envelope ping{
            .messageId = QStringLiteral("bootstrap-ping"),
            .sessionId = QStringLiteral("session-demo"),
            .sourceNodeId = nodeIdentityStore_.identity().nodeId,
            .targetNodeId = QStringLiteral("discovery"),
            .type = ControlMessageType::Ping,
            .payload = QJsonObject{{QStringLiteral("probe"), QStringLiteral("runtime")}},
    };
    ackTracker_.registerMessage(ping.messageId);
    logger_->info("Protocol bootstrap frame size={} bytes", messageCodec_.encode(ping).size());
    logger_->info("libsodium initialized, Opus runtime={}", opus_get_version_string());

    if (config_.enableDiscovery) {
        DiscoverySettings discoverySettings{
                .nodeId = nodeIdentityStore_.identity().nodeId,
                .displayName = nodeIdentityStore_.identity().displayName,
                .publicKey = nodeIdentityStore_.identity().publicKey,
                .capabilities = {QStringLiteral("control"), QStringLiteral("chat"), QStringLiteral("files"), QStringLiteral("voice")},
                .discoveryPort = config_.discoveryPort,
                .controlPort = config_.controlPort,
                .filePort = config_.filePort,
                .voicePort = config_.voicePort,
        };
        discoveryService_.configure(
                discoverySettings,
                &identityService_,
                [this](const PeerDescriptor &peer) { handlePeerUpdated(peer); },
                [this](const QString &message) {
                    if (logger_ != nullptr) {
                        logger_->info("{}", message.toStdString());
                    }
                });
        discoveryService_.seedPeers(peerRegistry_.all());
    }

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

PeersTableModel &DependencyContainer::peersTableModel()
{
    return peersTableModel_;
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

void DependencyContainer::refreshDiscoveryState()
{
    discoveryService_.refreshPeerStatuses();
    if (peerController_ != nullptr) {
        peerController_->refresh();
    }
}

void DependencyContainer::handlePeerUpdated(const PeerDescriptor &peer)
{
    trustStore_.setTrustLevel(peer.peerId, peer.trustLevel);
    peerRegistry_.upsert(peer);
    if (peerRepository_ != nullptr) {
        peerRepository_->save(peer);
    }
    if (peerController_ != nullptr) {
        peerController_->refresh();
    }
}
