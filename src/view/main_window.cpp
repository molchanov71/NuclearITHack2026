#include "main_window.hpp"

#include <QHBoxLayout>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "../app/dependency_container.hpp"

MainWindow::MainWindow(DependencyContainer &container, QWidget *parent)
    : QMainWindow(parent), container_(container)
{
    buildUi();
    container_.logger().info("Main window created");

    refreshTimer_ = new QTimer(this);
    refreshTimer_->setInterval(1000);
    connect(refreshTimer_, &QTimer::timeout, this, [this]() {
        refreshPeersView();
    });
    refreshTimer_->start();
}

void MainWindow::buildUi()
{
    setWindowTitle(container_.config().appName);
    resize(1200, 800);

    tabs_ = new QTabWidget(this);
    tabs_->addTab(createPeersPage(), QStringLiteral("Peers"));
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

QWidget *MainWindow::createPeersPage()
{
    auto *page = new QWidget(this);
    auto *layout = new QVBoxLayout(page);

    auto *titleLabel = new QLabel(QStringLiteral("Peers"), page);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: 700;");
    layout->addWidget(titleLabel);

    auto *descriptionLabel = new QLabel(
            QStringLiteral("Автоматически найденные LAN-узлы, их capability set, IP/ports и состояние TTL."),
            page);
    descriptionLabel->setWordWrap(true);
    layout->addWidget(descriptionLabel);

    auto *manualLayout = new QHBoxLayout();
    manualPeerInput_ = new QLineEdit(page);
    manualPeerInput_->setObjectName(QStringLiteral("manualPeerInput"));
    manualPeerInput_->setPlaceholderText(QStringLiteral("Добавить peer по IP, например 192.168.1.10"));
    auto *addButton = new QPushButton(QStringLiteral("Add peer by IP"), page);
    connect(addButton, &QPushButton::clicked, this, [this]() {
        container_.peerController().addManualPeer(manualPeerInput_->text());
        manualPeerInput_->clear();
        refreshPeersView();
    });
    manualLayout->addWidget(manualPeerInput_);
    manualLayout->addWidget(addButton);
    layout->addLayout(manualLayout);

    peersTable_ = new QTableView(page);
    peersTable_->setObjectName(QStringLiteral("peersTable"));
    peersTable_->setModel(&container_.peerController().tableModel());
    peersTable_->horizontalHeader()->setStretchLastSection(true);
    peersTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    peersTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    layout->addWidget(peersTable_);

    container_.peerController().refresh();
    return page;
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

    auto *listWidget = createListWidget(lines, page);
    layout->insertWidget(2, listWidget);

    return page;
}

QListWidget *MainWindow::createListWidget(const QStringList &lines, QWidget *parent) const
{
    auto *listWidget = new QListWidget(parent);
    listWidget->addItems(lines);
    return listWidget;
}

void MainWindow::refreshPeersView()
{
    container_.refreshDiscoveryState();
    if (peersTable_ != nullptr) {
        peersTable_->viewport()->update();
    }
}
