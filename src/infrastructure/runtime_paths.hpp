#pragma once

#include <QString>

/**
 * @brief Набор путей, которые приложение использует для хранения данных runtime.
 */
struct RuntimePaths
{
    QString dataDir;
    QString logDir;
    QString securityDir;
    QString transfersTmpDir;
};

/**
 * @brief Готовит базовые каталоги данных приложения.
 */
class RuntimePathsService
{
public:
    /**
     * @brief Создаёт каталоги данных, логов, ключей и временных файлов.
     * @param dataDir Корневой каталог данных приложения.
     * @return Подготовленная структура путей runtime.
     */
    [[nodiscard]] RuntimePaths initialize(const QString &dataDir) const;
};
