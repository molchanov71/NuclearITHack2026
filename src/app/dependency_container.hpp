#pragma once

#include <memory>

#include <spdlog/logger.h>

#include "app_config.hpp"

/**
 * @brief Хранит общие зависимости приложения и управляет их жизненным циклом.
 */
class DependencyContainer
{
public:
    /** @brief Создаёт пустой контейнер. Реальные сервисы поднимаются в initialize(). */
    DependencyContainer() = default;

    /**
     * @brief Инициализирует общие сервисы на основе переданной конфигурации.
     * @param config Конфигурация приложения, подготовленная runtime.
     */
    void initialize(const AppConfig &config);

    /** @brief Освобождает общие сервисы и сбрасывает состояние контейнера. */
    void shutdown();

    /**
     * @brief Возвращает активную конфигурацию приложения.
     * @return Константная ссылка на конфигурацию, хранящуюся в контейнере.
     */
    [[nodiscard]] const AppConfig &config() const;

    /**
     * @brief Возвращает общий логгер приложения.
     * @return Экземпляр логгера, принадлежащий контейнеру.
     */
    [[nodiscard]] spdlog::logger &logger() const;

    /**
     * @brief Показывает, завершилась ли initialize() успешно.
     * @return true, если общие сервисы готовы к использованию.
     */
    [[nodiscard]] bool isInitialized() const;

private:
    /** @brief Сохранённая конфигурация приложения. */
    AppConfig config_{};
    /** @brief Общий экземпляр логгера runtime. */
    std::shared_ptr<spdlog::logger> logger_{};
    /** @brief Флаг, показывающий, что контейнер содержит активные сервисы. */
    bool initialized_ = false;
};
