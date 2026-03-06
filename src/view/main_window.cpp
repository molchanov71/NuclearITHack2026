#include "main_window.hpp"

#include <QLabel>
#include <QListWidget>
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
    tabs_->addTab(createListPage(QStringLiteral("Peers"),
                                 QStringLiteral("Known peers, discovery status and connection health."),
                                 container_.peerController().peerLines()),
                  QStringLiteral("Peers"));
    tabs_->addTab(createListPage(QStringLiteral("Chat"),
                                 QStringLiteral("Direct and group chat history will be shown here."),
                                 container_.chatController().chatLines()),
                  QStringLiteral("Chat"));
    tabs_->addTab(createListPage(QStringLiteral("Files"),
                                 QStringLiteral("Secure file transfers and queues will be shown here."),
                                 container_.fileTransferController().transferLines()),
                  QStringLiteral("Files"));
    tabs_->addTab(createListPage(QStringLiteral("Calls"),
                                 QStringLiteral("Voice call controls and diagnostics will live here."),
                                 container_.callController().callLines()),
                  QStringLiteral("Calls"));
    QStringList diagnostics = container_.appController().summaryLines();
    diagnostics.append(container_.sessionController().sessionLines());
    diagnostics.append(container_.diagnosticsController().diagnosticLines());
    diagnostics.append(container_.uiLogBuffer().lines());
    tabs_->addTab(createListPage(QStringLiteral("Diagnostics"),
                                 QStringLiteral("Runtime flags, logs and service status will be shown here."),
                                 diagnostics),
                  QStringLiteral("Diagnostics"));

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

QWidget *MainWindow::createListPage(const QString &title,
                                    const QString &description,
                                    const QStringList &lines) const
{
    auto *page = createPlaceholderPage(title, description);
    auto *layout = qobject_cast<QVBoxLayout *>(page->layout());

    auto *listWidget = new QListWidget(page);
    listWidget->addItems(lines);
    layout->insertWidget(2, listWidget);

    return page;
}
