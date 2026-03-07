#pragma once

#include <QMainWindow>
#include <QStringList>
#include <QString>

class DependencyContainer;
class QLineEdit;
class QListWidget;
class QTabWidget;
class QTableView;
class QTimer;

/**
 * @brief Главное окно приложения с вкладками-заглушками для основных функций.
 */
class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Создаёт главное окно с использованием общих зависимостей приложения.
     * @param container Общий контейнер зависимостей, подготовленный runtime.
     * @param parent Необязательный родительский Qt-виджет.
     */
    explicit MainWindow(DependencyContainer &container, QWidget *parent = nullptr);

private:
    /** @brief Собирает интерфейс с вкладками-заглушками. */
    void buildUi();
    QWidget *createPeersPage();
    void refreshPeersView();

    /**
     * @brief Создаёт простую страницу с заголовком и описанием.
     * @param title Отображаемый заголовок страницы.
     * @param description Текстовое описание-заглушка.
     * @return Новый виджет страницы, владение которым передаётся Qt.
     */
    QWidget *createPlaceholderPage(const QString &title, const QString &description) const;
    QWidget *createListPage(const QString &title,
                            const QString &description,
                            const QStringList &lines) const;
    QListWidget *createListWidget(const QStringList &lines, QWidget *parent) const;

    /** @brief Общие зависимости, используемые окном. */
    DependencyContainer &container_;
    /** @brief Центральный таб-виджет с временными страницами. */
    QTabWidget *tabs_ = nullptr;
    /** @brief Таблица найденных peers. */
    QTableView *peersTable_ = nullptr;
    /** @brief Поле ручного добавления peer по IP. */
    QLineEdit *manualPeerInput_ = nullptr;
    /** @brief Таймер периодического обновления discovery UI. */
    QTimer *refreshTimer_ = nullptr;
};
