#include "protocol.hpp"

#include <QDataStream>
#include <QIODevice>
#include <QJsonDocument>

QByteArray MessageCodec::encode(const Envelope &envelope) const
{
    QJsonObject object{
            {QStringLiteral("messageId"), envelope.messageId},
            {QStringLiteral("sessionId"), envelope.sessionId},
            {QStringLiteral("sourceNodeId"), envelope.sourceNodeId},
            {QStringLiteral("targetNodeId"), envelope.targetNodeId},
            {QStringLiteral("type"), typeToString(envelope.type)},
            {QStringLiteral("payload"), envelope.payload},
    };

    const QByteArray json = QJsonDocument(object).toJson(QJsonDocument::Compact);
    QByteArray frame;
    QDataStream stream(&frame, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << static_cast<quint32>(json.size());
    frame.append(json);
    return frame;
}

Envelope MessageCodec::decode(const QByteArray &frame) const
{
    QDataStream stream(frame);
    stream.setByteOrder(QDataStream::BigEndian);

    quint32 size = 0;
    stream >> size;
    const QByteArray payload = frame.mid(static_cast<int>(sizeof(quint32)), static_cast<int>(size));
    const QJsonObject object = QJsonDocument::fromJson(payload).object();

    return Envelope{
            .messageId = object.value(QStringLiteral("messageId")).toString(),
            .sessionId = object.value(QStringLiteral("sessionId")).toString(),
            .sourceNodeId = object.value(QStringLiteral("sourceNodeId")).toString(),
            .targetNodeId = object.value(QStringLiteral("targetNodeId")).toString(),
            .type = stringToType(object.value(QStringLiteral("type")).toString()),
            .payload = object.value(QStringLiteral("payload")).toObject(),
    };
}

QString MessageCodec::typeToString(ControlMessageType type) const
{
    switch (type) {
    case ControlMessageType::Announce:
        return QStringLiteral("ANNOUNCE");
    case ControlMessageType::SessionHello:
        return QStringLiteral("SESSION_HELLO");
    case ControlMessageType::ChatMessage:
        return QStringLiteral("CHAT_MSG");
    case ControlMessageType::ChatAck:
        return QStringLiteral("CHAT_ACK");
    case ControlMessageType::Ping:
        return QStringLiteral("PING");
    case ControlMessageType::Pong:
        return QStringLiteral("PONG");
    case ControlMessageType::CallInvite:
        return QStringLiteral("CALL_INVITE");
    case ControlMessageType::FileOffer:
        return QStringLiteral("FILE_OFFER");
    case ControlMessageType::Error:
        return QStringLiteral("ERROR");
    }

    return QStringLiteral("ERROR");
}

ControlMessageType MessageCodec::stringToType(const QString &type) const
{
    if (type == QStringLiteral("ANNOUNCE")) {
        return ControlMessageType::Announce;
    }
    if (type == QStringLiteral("SESSION_HELLO")) {
        return ControlMessageType::SessionHello;
    }
    if (type == QStringLiteral("CHAT_MSG")) {
        return ControlMessageType::ChatMessage;
    }
    if (type == QStringLiteral("CHAT_ACK")) {
        return ControlMessageType::ChatAck;
    }
    if (type == QStringLiteral("PING")) {
        return ControlMessageType::Ping;
    }
    if (type == QStringLiteral("PONG")) {
        return ControlMessageType::Pong;
    }
    if (type == QStringLiteral("CALL_INVITE")) {
        return ControlMessageType::CallInvite;
    }
    if (type == QStringLiteral("FILE_OFFER")) {
        return ControlMessageType::FileOffer;
    }

    return ControlMessageType::Error;
}

bool AckTracker::registerMessage(const QString &messageId)
{
    if (seenMessages_.contains(messageId)) {
        return false;
    }

    seenMessages_.insert(messageId, true);
    return true;
}
