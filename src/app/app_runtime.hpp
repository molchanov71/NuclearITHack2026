#pragma once

#include <memory>

#include <QApplication>

#include "app_config.hpp"
#include "dependency_container.hpp"

/**
 * @brief Управляет запуском приложения, работой event loop и завершением.
 */
class AppRuntime
{
public:
    /**
     * @brief Создаёт runtime-обёртку над аргументами командной строки.
     * @param argc Количество аргументов командной строки.
     * @param argv Массив аргументов командной строки.
     */
    AppRuntime(int &argc, char **argv);

    /**
     * @brief Запускает приложение и возвращает итоговый код завершения.
     * @return Код завершения Qt event loop.
     */
    int run();

private:
    /** @brief Создаёт Qt-приложение, конфигурацию и общие сервисы. */
    void initialize();
    /** @brief Останавливает общие сервисы перед завершением процесса. */
    void shutdown();

    /** @brief Количество аргументов командной строки для QApplication. */
    int &argc_;
    /** @brief Значения аргументов командной строки для QApplication. */
    char **argv_;
    /** @brief Экземпляр Qt-приложения, владеющий основным event loop. */
    std::unique_ptr<QApplication> application_;
    /** @brief Конфигурация, загруженная при запуске. */
    AppConfig config_{};
    /** @brief Контейнер общих зависимостей, используемый UI и сервисами. */
    DependencyContainer container_{};
};
