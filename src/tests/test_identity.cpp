#include <QtTest>

#include <QDir>
#include <QTemporaryDir>

#include "../infrastructure/identity_service.hpp"

class IdentityServiceTest final : public QObject
{
    Q_OBJECT

private slots:
    void generatedIdentityIsStableOnDisk();
    void signatureRoundTripWorks();
};

void IdentityServiceTest::generatedIdentityIsStableOnDisk()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    IdentityService firstService;
    const IdentityMaterial first = firstService.initialize(dir.path());

    IdentityService secondService;
    const IdentityMaterial second = secondService.initialize(dir.path());

    QCOMPARE(first.publicKey, second.publicKey);
    QCOMPARE(first.secretKey, second.secretKey);
    QCOMPARE(first.nodeId, second.nodeId);
}

void IdentityServiceTest::signatureRoundTripWorks()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    IdentityService service;
    const IdentityMaterial material = service.initialize(dir.path());
    const QByteArray payload("secure-payload");
    const QByteArray signature = service.sign(payload);

    QVERIFY(service.verify(payload, signature, material.publicKey));
}

QTEST_APPLESS_MAIN(IdentityServiceTest)

#include "test_identity.moc"
