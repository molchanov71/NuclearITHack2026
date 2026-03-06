#include "dependency_container.hpp"

#include <QDir>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void DependencyContainer::initialize(const AppConfig &config)
{
    config_ = config;

    QDir().mkpath(config_.dataDir);
    QDir().mkpath(config_.logDir);

    logger_ = spdlog::stdout_color_mt("app");
    logger_->set_pattern("[%H:%M:%S] [%^%l%$] %v");
    logger_->info("Runtime initialized");

    initialized_ = true;
}

void DependencyContainer::shutdown()
{
    if (logger_ != nullptr) {
        logger_->info("Runtime shutdown");
    }

    spdlog::drop("app");
    logger_.reset();
    initialized_ = false;
}

const AppConfig &DependencyContainer::config() const
{
    return config_;
}

spdlog::logger &DependencyContainer::logger() const
{
    return *logger_;
}

bool DependencyContainer::isInitialized() const
{
    return initialized_;
}
