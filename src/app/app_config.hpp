#pragma once

#include <QString>
#include <QtGlobal>

/**
 * @brief Хранит базовые настройки приложения, используемые при запуске.
 */
struct AppConfig
{
    /** @brief Человекочитаемое имя приложения. */
    QString appName;
    /** @brief Имя организации, используемое Qt для настроек и путей. */
    QString organizationName;
    /** @brief Корневая директория для данных приложения. */
    QString dataDir;
    /** @brief Директория для логов приложения. */
    QString logDir;
    /** @brief Полный путь к файлу базы данных приложения. */
    QString dbPath;
    /** @brief UDP-порт, зарезервированный для обнаружения узлов. */
    quint16 discoveryPort;
    /** @brief Порт, зарезервированный для чата. */
    quint16 chatPort;
    /** @brief Флаг включения сервиса обнаружения узлов. */
    bool enableDiscovery;
    /** @brief Флаг включения логирования. */
    bool enableLogging;

    /**
     * @brief Создаёт конфигурацию по умолчанию для локального запуска.
     * @return Заполненная структура конфигурации приложения.
     */
    static AppConfig loadDefault();
};
