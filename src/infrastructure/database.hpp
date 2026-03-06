#pragma once

#include <QSqlDatabase>

/**
 * @brief Открывает SQLite и инициализирует базовую схему приложения.
 */
class Database
{
public:
    Database();
    ~Database();

    /**
     * @brief Подключает локальную SQLite базу и при необходимости создаёт схему.
     * @param path Полный путь к файлу базы данных.
     * @return true, если база успешно открыта и схема готова.
     */
    bool initialize(const QString &path);

    /** @brief Закрывает подключение к базе данных. */
    void shutdown();

    /** @brief Возвращает активное Qt SQL подключение. */
    [[nodiscard]] const QSqlDatabase &connection() const;

private:
    bool initializeSchema();
    QSqlDatabase database_{};
    QString connectionName_{};
};
