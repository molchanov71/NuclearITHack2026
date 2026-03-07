#include <QtTest>

#include <QFileInfo>
#include <QTemporaryDir>

#include "../app/app_config.hpp"
#include "../app/dependency_container.hpp"

class RuntimeSmokeTest final : public QObject
{
    Q_OBJECT

private slots:
    void runtimeCreatesDataAndOpensDatabase();
};

void RuntimeSmokeTest::runtimeCreatesDataAndOpensDatabase()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    AppConfig config = AppConfig::loadDefault();
    config.dataDir = dir.path();
    config.logDir = dir.path() + QStringLiteral("/logs");
    config.securityDir = dir.path() + QStringLiteral("/security");
    config.transfersTmpDir = dir.path() + QStringLiteral("/transfers/tmp");
    config.dbPath = dir.path() + QStringLiteral("/app.db");
    config.enableNetwork = false;

    DependencyContainer container;
    container.initialize(config);

    QVERIFY(container.isInitialized());
    QVERIFY(QFileInfo::exists(config.dbPath));
    QVERIFY(QFileInfo::exists(config.securityDir + QStringLiteral("/identity_ed25519.pub")));
    QVERIFY(QFileInfo::exists(config.logDir + QStringLiteral("/runtime.log")));
    QVERIFY(container.database().connection().isOpen());
    QCOMPARE(container.peerRepository().loadAll().size(), 0);
    QCOMPARE(container.messageRepository().loadAll().size(), 0);
    QCOMPARE(container.fileManifestRepository().loadAll().size(), 0);
    QCOMPARE(container.transferRepository().loadAll().size(), 0);
    QCOMPARE(container.metricsRepository().loadAll().size(), 0);

    container.shutdown();
}

QTEST_APPLESS_MAIN(RuntimeSmokeTest)

#include "test_runtime_smoke.moc"
