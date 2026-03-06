#include <QtTest>

#include <QJsonObject>

#include "../infrastructure/protocol.hpp"

class ProtocolTest final : public QObject
{
    Q_OBJECT

private slots:
    void roundTripPreservesEnvelope();
    void ackTrackerRejectsDuplicates();
};

void ProtocolTest::roundTripPreservesEnvelope()
{
    MessageCodec codec;
    const Envelope envelope{
            .messageId = QStringLiteral("msg-1"),
            .sessionId = QStringLiteral("session-1"),
            .sourceNodeId = QStringLiteral("node-a"),
            .targetNodeId = QStringLiteral("node-b"),
            .type = ControlMessageType::Ping,
            .payload = QJsonObject{{QStringLiteral("seq"), 42}},
    };

    const QByteArray frame = codec.encode(envelope);
    const Envelope decoded = codec.decode(frame);

    QCOMPARE(decoded.messageId, envelope.messageId);
    QCOMPARE(decoded.sessionId, envelope.sessionId);
    QCOMPARE(decoded.sourceNodeId, envelope.sourceNodeId);
    QCOMPARE(decoded.targetNodeId, envelope.targetNodeId);
    QCOMPARE(decoded.payload.value(QStringLiteral("seq")).toInt(), 42);
    QCOMPARE(decoded.type, ControlMessageType::Ping);
}

void ProtocolTest::ackTrackerRejectsDuplicates()
{
    AckTracker tracker;
    QVERIFY(tracker.registerMessage(QStringLiteral("same-id")));
    QVERIFY(!tracker.registerMessage(QStringLiteral("same-id")));
}

QTEST_APPLESS_MAIN(ProtocolTest)

#include "test_protocol.moc"
