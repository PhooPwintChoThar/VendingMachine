// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QStackedWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlTableModel>
#include <QHeaderView>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // GUI Elements
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    QWidget *mainPage;
    QWidget *adminPage;
    QWidget *userPage;
    QTableWidget *stockTable;
    QTableWidget *changeBoxTable;
    QTableWidget *collectionBoxTable;
    QTableWidget *itemsTable;

    // Database Models
    QSqlTableModel *stockModel;
    QSqlTableModel *changeBoxModel;
    QSqlTableModel *collectionBoxModel;

    // Methods
    void setupUi();
    void createMainPage();
    void createAdminPage();
    void createUserPage();
    void refreshTables();
    bool checkOperatingConditions();
    void processPayment(const QString &itemName, int price);
    void calculateChange(int changeAmount);
    void updateStock(const QString &itemName);

private slots:
    void showAdminMode();
    void showUserMode();
    void addNewItem();
    void deleteItem();
    void restockItem();
    void refillChange();
    void collectMoney();
    void handleItemPurchase();
    void returnToMain();
};

#endif // MAINWINDOW_H

