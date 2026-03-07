#include <QtTest>

#include <QTabWidget>
#include <QTemporaryDir>

#include "../app/app_config.hpp"
#include "../app/dependency_container.hpp"
#include "../view/main_window.hpp"

class ViewSmokeTest final : public QObject
{
    Q_OBJECT

private slots:
    void mainWindowBuildsExpectedTabs();
};

void ViewSmokeTest::mainWindowBuildsExpectedTabs()
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

    MainWindow window(container);
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));

    auto *tabs = window.findChild<QTabWidget *>();
    QVERIFY(tabs != nullptr);
    QCOMPARE(tabs->count(), 5);
    QCOMPARE(tabs->tabText(0), QStringLiteral("Peers"));
    QCOMPARE(tabs->tabText(4), QStringLiteral("Diagnostics"));

    container.shutdown();
}

QTEST_MAIN(ViewSmokeTest)

#include "test_view_smoke.moc"
