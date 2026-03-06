#pragma once

#include <memory>

#include <QStringList>

#include <spdlog/logger.h>

/**
 * @brief Хранит небольшой буфер последних лог-сообщений для UI Diagnostics.
 */
class UiLogBuffer
{
public:
    void append(const QString &line);
    [[nodiscard]] QStringList lines() const;

private:
    QStringList lines_{};
};

/**
 * @brief Поднимает файловое и консольное логирование для runtime.
 */
class LogService
{
public:
    /**
     * @brief Создаёт логгер приложения.
     * @param logDir Каталог для файлов логов.
     * @return Готовый логгер spdlog.
     */
    [[nodiscard]] std::shared_ptr<spdlog::logger> initialize(const QString &logDir);

    /**
     * @brief Возвращает буфер строк для отображения в UI.
     * @return Буфер последних логов.
     */
    [[nodiscard]] UiLogBuffer &uiBuffer();
    [[nodiscard]] const UiLogBuffer &uiBuffer() const;

private:
    UiLogBuffer uiBuffer_{};
};
