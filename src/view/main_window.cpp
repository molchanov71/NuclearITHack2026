#include "main_window.hpp"

#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "../app/dependency_container.hpp"

MainWindow::MainWindow(DependencyContainer &container, QWidget *parent)
    : QMainWindow(parent), container_(container)
{
    buildUi();
    container_.logger().info("Main window created");
}

void MainWindow::buildUi()
{
    setWindowTitle(container_.config().appName);
    resize(1200, 800);

    tabs_ = new QTabWidget(this);
    tabs_->addTab(createPlaceholderPage("Peers", "Known peers, discovery status and connection health."), "Peers");
    tabs_->addTab(createPlaceholderPage("Chat", "Direct and group chat history will be shown here."), "Chat");
    tabs_->addTab(createPlaceholderPage("Files", "Secure file transfers and queues will be shown here."), "Files");
    tabs_->addTab(createPlaceholderPage("Calls", "Voice call controls and diagnostics will live here."), "Calls");
    tabs_->addTab(createPlaceholderPage("Diagnostics", "Runtime flags, logs and service status will be shown here."), "Diagnostics");

    setCentralWidget(tabs_);
}

QWidget *MainWindow::createPlaceholderPage(const QString &title, const QString &description) const
{
    auto *page = new QWidget();
    auto *layout = new QVBoxLayout(page);

    auto *titleLabel = new QLabel(title, page);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: 700;");

    auto *descriptionLabel = new QLabel(description, page);
    descriptionLabel->setWordWrap(true);

    layout->addWidget(titleLabel);
    layout->addWidget(descriptionLabel);
    layout->addStretch();

    return page;
}
