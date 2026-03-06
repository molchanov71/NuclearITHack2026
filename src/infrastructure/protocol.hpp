#pragma once

#include <QByteArray>
#include <QHash>
#include <QJsonObject>
#include <QString>

/**
 * @brief Тип control-сообщения transport envelope.
 */
enum class ControlMessageType
{
    Announce,
    SessionHello,
    ChatMessage,
    ChatAck,
    Ping,
    Pong,
    CallInvite,
    FileOffer,
    Error
};

/**
 * @brief Единый envelope для control plane сообщений.
 */
struct Envelope
{
    QString messageId;
    QString sessionId;
    QString sourceNodeId;
    QString targetNodeId;
    ControlMessageType type = ControlMessageType::Error;
    QJsonObject payload;
};

/**
 * @brief Кодирует и декодирует envelope в формате JSON c length prefix.
 */
class MessageCodec
{
public:
    [[nodiscard]] QByteArray encode(const Envelope &envelope) const;
    [[nodiscard]] Envelope decode(const QByteArray &frame) const;

private:
    [[nodiscard]] QString typeToString(ControlMessageType type) const;
    [[nodiscard]] ControlMessageType stringToType(const QString &type) const;
};

/**
 * @brief Отслеживает обработанные messageId и не даёт повторно принимать дубликаты.
 */
class AckTracker
{
public:
    /** @brief Возвращает true, если messageId встречен впервые. */
    bool registerMessage(const QString &messageId);

private:
    QHash<QString, bool> seenMessages_{};
};
