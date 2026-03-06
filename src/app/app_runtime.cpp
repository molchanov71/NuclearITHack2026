#include "app_runtime.hpp"

#include <QApplication>

#include "../view/main_window.hpp"

AppRuntime::AppRuntime(int &argc, char **argv)
    : argc_(argc), argv_(argv)
{
}

int AppRuntime::run()
{
    initialize();

    MainWindow window(container_);
    window.show();

    const int exitCode = application_->exec();
    shutdown();

    return exitCode;
}

void AppRuntime::initialize()
{
    application_ = std::make_unique<QApplication>(argc_, argv_);

    config_ = AppConfig::loadDefault();
    application_->setApplicationName(config_.appName);
    application_->setOrganizationName(config_.organizationName);

    container_.initialize(config_);
}

void AppRuntime::shutdown()
{
    if (container_.isInitialized()) {
        container_.shutdown();
    }
}
