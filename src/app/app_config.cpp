#include "app_config.hpp"

#include <QByteArray>
#include <QDir>
#include <QStandardPaths>

AppConfig AppConfig::loadDefault()
{
    const QString appName = QStringLiteral("NuclearITHack2026");
    QString dataDir = QString::fromLocal8Bit(qgetenv("NUCLEARIT_DATA_DIR"));
    if (dataDir.isEmpty()) {
        dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    if (dataDir.isEmpty()) {
        dataDir = QDir::currentPath() + QStringLiteral("/data");
    }

    const QString logDir = dataDir + QStringLiteral("/logs");
    const QString securityDir = dataDir + QStringLiteral("/security");
    const QString transfersTmpDir = dataDir + QStringLiteral("/transfers/tmp");

    return AppConfig{
            .appName = appName,
            .organizationName = QStringLiteral("NuclearITHack"),
            .dataDir = dataDir,
            .logDir = logDir,
            .securityDir = securityDir,
            .transfersTmpDir = transfersTmpDir,
            .dbPath = dataDir + QStringLiteral("/app.db"),
            .discoveryPort = 45454,
            .controlPort = 45455,
            .filePort = 45456,
            .voicePort = 45457,
            .enableDiscovery = true,
            .enableLogging = true,
            .enableNetwork = true,
    };
}
