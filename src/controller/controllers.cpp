#include "controllers.hpp"

#include <QDateTime>

#include "../app/app_config.hpp"

namespace
{
QString formatTimestamp(const QDateTime &timestamp)
{
    return timestamp.isValid() ? timestamp.toString(Qt::ISODate) : QStringLiteral("n/a");
}
}

AppController::AppController(const AppConfig &config, const MetricsStore &metricsStore)
    : config_(config), metricsStore_(metricsStore)
{
}

QStringList AppController::summaryLines() const
{
    return {
            QStringLiteral("Имя узла: %1").arg(config_.appName),
            QStringLiteral("Каталог данных: %1").arg(config_.dataDir),
            QStringLiteral("Метрик собрано: %1").arg(metricsStore_.size()),
    };
}

PeerController::PeerController(const PeerRegistryModel &peerRegistry)
    : peerRegistry_(peerRegistry)
{
}

QStringList PeerController::peerLines() const
{
    QStringList lines;
    for (const PeerDescriptor &peer : peerRegistry_.all()) {
        lines.append(QStringLiteral("%1 [%2] last seen %3")
                             .arg(peer.displayName, peer.peerId, formatTimestamp(peer.lastSeenAt)));
    }

    if (lines.isEmpty()) {
        lines.append(QStringLiteral("Пиры ещё не обнаружены."));
    }

    return lines;
}

SessionController::SessionController(const SessionStore &sessionStore)
    : sessionStore_(sessionStore)
{
}

QStringList SessionController::sessionLines() const
{
    QStringList lines;
    for (const Session &session : sessionStore_.all()) {
        lines.append(QStringLiteral("%1 -> %2, encrypted=%3, activity=%4")
                             .arg(session.sessionId,
                                  session.peerId,
                                  session.encrypted ? QStringLiteral("yes") : QStringLiteral("no"),
                                  formatTimestamp(session.lastActivityAt)));
    }

    if (lines.isEmpty()) {
        lines.append(QStringLiteral("Сессий пока нет."));
    }

    return lines;
}

ChatController::ChatController(const MessageStore &messageStore)
    : messageStore_(messageStore)
{
}

QStringList ChatController::chatLines() const
{
    QStringList lines;
    for (const ChatMessage &message : messageStore_.all()) {
        lines.append(QStringLiteral("%1: %2").arg(message.authorId, message.text));
    }

    if (lines.isEmpty()) {
        lines.append(QStringLiteral("История сообщений пуста."));
    }

    return lines;
}

FileTransferController::FileTransferController(const TransferStore &transferStore)
    : transferStore_(transferStore)
{
}

QStringList FileTransferController::transferLines() const
{
    QStringList lines;
    for (const FileTransfer &transfer : transferStore_.transfers()) {
        lines.append(QStringLiteral("%1: %2 / %3 bytes")
                             .arg(transfer.transferId)
                             .arg(transfer.bytesTransferred)
                             .arg(transfer.bytesTotal));
    }

    if (lines.isEmpty()) {
        lines.append(QStringLiteral("Передач файлов пока нет."));
    }

    return lines;
}

CallController::CallController(const MetricsStore &metricsStore)
    : metricsStore_(metricsStore)
{
}

QStringList CallController::callLines() const
{
    if (metricsStore_.size() == 0) {
        return {QStringLiteral("Голосовые вызовы ещё не запускались.")};
    }

    return {QStringLiteral("Сетевые метрики готовы, можно подключать voice pipeline.")};
}

DiagnosticsController::DiagnosticsController(const NodeIdentityStore &identityStore,
                                             const MetricsStore &metricsStore,
                                             const TrustStoreModel &trustStore)
    : identityStore_(identityStore), metricsStore_(metricsStore), trustStore_(trustStore)
{
}

QStringList DiagnosticsController::diagnosticLines() const
{
    QStringList lines;
    if (identityStore_.hasIdentity()) {
        lines.append(QStringLiteral("NodeId: %1").arg(identityStore_.identity().nodeId));
        lines.append(QStringLiteral("Identity created: %1").arg(formatTimestamp(identityStore_.identity().createdAt)));
    } else {
        lines.append(QStringLiteral("Идентичность ещё не инициализирована."));
    }

    lines.append(QStringLiteral("Snapshots: %1").arg(metricsStore_.size()));
    lines.append(QStringLiteral("Trusted peers: %1").arg(trustStore_.size()));

    return lines;
}
