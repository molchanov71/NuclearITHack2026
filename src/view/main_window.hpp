#pragma once

#include <QMainWindow>
#include <QStringList>
#include <QString>

class DependencyContainer;
class QTabWidget;

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

    /** @brief Общие зависимости, используемые окном. */
    DependencyContainer &container_;
    /** @brief Центральный таб-виджет с временными страницами. */
    QTabWidget *tabs_ = nullptr;
};
