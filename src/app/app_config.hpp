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
    /** @brief Директория для security-материалов узла. */
    QString securityDir;
    /** @brief Директория для временных данных file transfer. */
    QString transfersTmpDir;
    /** @brief Полный путь к файлу базы данных приложения. */
    QString dbPath;
    /** @brief UDP-порт, зарезервированный для обнаружения узлов. */
    quint16 discoveryPort;
    /** @brief TCP-порт control plane. */
    quint16 controlPort;
    /** @brief TCP-порт для файлового канала. */
    quint16 filePort;
    /** @brief UDP-порт голосового канала. */
    quint16 voicePort;
    /** @brief Флаг включения сервиса обнаружения узлов. */
    bool enableDiscovery;
    /** @brief Флаг включения логирования. */
    bool enableLogging;
    /** @brief Флаг включения сетевых заглушек. */
    bool enableNetwork;

    /**
     * @brief Создаёт конфигурацию по умолчанию для локального запуска.
     * @return Заполненная структура конфигурации приложения.
     */
    static AppConfig loadDefault();
};
