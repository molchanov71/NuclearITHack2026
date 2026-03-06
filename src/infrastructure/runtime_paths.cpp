#include "runtime_paths.hpp"

#include <QDir>

RuntimePaths RuntimePathsService::initialize(const QString &dataDir) const
{
    RuntimePaths paths{
            .dataDir = dataDir,
            .logDir = dataDir + QStringLiteral("/logs"),
            .securityDir = dataDir + QStringLiteral("/security"),
            .transfersTmpDir = dataDir + QStringLiteral("/transfers/tmp"),
    };

    QDir().mkpath(paths.dataDir);
    QDir().mkpath(paths.logDir);
    QDir().mkpath(paths.securityDir);
    QDir().mkpath(paths.transfersTmpDir);

    return paths;
}
