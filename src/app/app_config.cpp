#include "app_config.hpp"

#include <QDir>
#include <QStandardPaths>

AppConfig AppConfig::loadDefault()
{
    const QString appName = QStringLiteral("NuclearITHack2026");
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dataDir.isEmpty()) {
        dataDir = QDir::currentPath() + QStringLiteral("/data");
    }

    const QString logDir = dataDir + QStringLiteral("/logs");

    return AppConfig{
            .appName = appName,
            .organizationName = QStringLiteral("NuclearITHack"),
            .dataDir = dataDir,
            .logDir = logDir,
            .dbPath = dataDir + QStringLiteral("/app.db"),
            .discoveryPort = 45454,
            .chatPort = 45455,
            .enableDiscovery = true,
            .enableLogging = true,
    };
}
