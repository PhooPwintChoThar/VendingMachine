// mainwindow.cpp
#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QFont>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Initialize the window with a title and reasonable size
    setWindowTitle("Modern Vending Machine");
    resize(1024, 768);

    // Create our central widget and main layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Initialize the stacked widget for different pages
    stackedWidget = new QStackedWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(stackedWidget);

    // Create all our pages
    createMainPage();
    createAdminPage();
    createUserPage();

    // Add pages to the stacked widget
    stackedWidget->addWidget(mainPage);
    stackedWidget->addWidget(adminPage);
    stackedWidget->addWidget(userPage);

    // Refresh all data tables
    refreshTables();
}

void MainWindow::createMainPage() {
    mainPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(mainPage);

    // Create and style the title label
    QLabel *titleLabel = new QLabel("Welcome to Smart Vending Machine");
    QFont titleFont("Arial", 24, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #ECF0F1; margin: 20px;");

    // Create mode selection buttons
    QPushButton *userButton = new QPushButton("User Mode");
    QPushButton *adminButton = new QPushButton("Admin Mode");

    // Style the buttons for a modern look
    QString buttonStyle =
        "QPushButton {"
        "    padding: 15px;"
        "    font-size: 18px;"
        "    border-radius: 8px;"
        "    min-width: 200px;"
        "    background-color: #2ECC71;"
        "    color: white;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #27AE60;"
        "}";

    userButton->setStyleSheet(buttonStyle);
    adminButton->setStyleSheet(buttonStyle);

    // Add widgets to layout with proper spacing
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(40);
    layout->addWidget(userButton, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(adminButton, 0, Qt::AlignCenter);
    layout->addStretch();

    // Connect buttons to their respective slots
    connect(userButton, &QPushButton::clicked, this, &MainWindow::showUserMode);
    connect(adminButton, &QPushButton::clicked, this, &MainWindow::showAdminMode);
}

void MainWindow::createAdminPage() {
    adminPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(adminPage);

    // Create and style the admin page title
    QLabel *titleLabel = new QLabel("Admin Control Panel");
    QFont titleFont("Arial", 20, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #ECF0F1; margin: 20px;");

    // Create tables for displaying data
    stockTable = new QTableWidget();
    stockTable->setColumnCount(3);
    stockTable->setHorizontalHeaderLabels({"Item Name", "Price (THB)", "Stock"});
    stockTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    changeBoxTable = new QTableWidget();
    changeBoxTable->setColumnCount(2);
    changeBoxTable->setHorizontalHeaderLabels({"Denomination", "Count"});
    changeBoxTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    collectionBoxTable = new QTableWidget();
    collectionBoxTable->setColumnCount(2);
    collectionBoxTable->setHorizontalHeaderLabels({"Denomination", "Count"});
    collectionBoxTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Create admin control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("Add Item");
    QPushButton *deleteButton = new QPushButton("Delete Item");
    QPushButton *restockButton = new QPushButton("Restock");
    QPushButton *refillChangeButton = new QPushButton("Refill Change");
    QPushButton *collectMoneyButton = new QPushButton("Collect Money");
    QPushButton *backButton = new QPushButton("Back to Main");

    // Style the buttons
    QString buttonStyle =
        "QPushButton {"
        "    padding: 10px;"
        "    border-radius: 5px;"
        "    background-color: #2ECC71;"
        "    color: white;"
        "    border: none;"
        "    min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #27AE60;"
        "}";

    addButton->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);
    restockButton->setStyleSheet(buttonStyle);
    refillChangeButton->setStyleSheet(buttonStyle);
    collectMoneyButton->setStyleSheet(buttonStyle);
    backButton->setStyleSheet(buttonStyle);

    // Add buttons to layout
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(restockButton);
    buttonLayout->addWidget(refillChangeButton);
    buttonLayout->addWidget(collectMoneyButton);
    buttonLayout->addWidget(backButton);

    // Create section labels
    QLabel *stockLabel = new QLabel("Stock Management");
    QLabel *changeLabel = new QLabel("Change Box Status");
    QLabel *collectionLabel = new QLabel("Collection Box Status");

    // Add all widgets to main layout
    layout->addWidget(titleLabel);
    layout->addWidget(stockLabel);
    layout->addWidget(stockTable);
    layout->addWidget(changeLabel);
    layout->addWidget(changeBoxTable);
    layout->addWidget(collectionLabel);
    layout->addWidget(collectionBoxTable);
    layout->addLayout(buttonLayout);

    // Connect buttons to their respective slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addNewItem);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteItem);
    connect(restockButton, &QPushButton::clicked, this, &MainWindow::restockItem);
    connect(refillChangeButton, &QPushButton::clicked, this, &MainWindow::refillChange);
    connect(collectMoneyButton, &QPushButton::clicked, this, &MainWindow::collectMoney);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::returnToMain);
}

void MainWindow::createUserPage() {
    userPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(userPage);

    // Create title for user page
    QLabel *titleLabel = new QLabel("Available Items");
    QFont titleFont("Arial", 20, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Create items display table
    itemsTable = new QTableWidget(); // Change to class member
    itemsTable->setColumnCount(3);
    itemsTable->setHorizontalHeaderLabels({"Item Name", "Price (THB)", "Stock"});
    itemsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add purchase button
    QPushButton *purchaseButton = new QPushButton("Purchase Selected Item");
    purchaseButton->setStyleSheet(
        "QPushButton {"
        "    padding: 10px;"
        "    border-radius: 5px;"
        "    background-color: #2ECC71;"
        "    color: white;"
        "    border: none;"
        "    min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #27AE60;"
        "}"
        );

    // Create back button
    QPushButton *backButton = new QPushButton("Back to Main");
    backButton->setStyleSheet(
        "QPushButton {"
        "    padding: 10px;"
        "    border-radius: 5px;"
        "    background-color: #2ECC71;"
        "    color: white;"
        "    border: none;"
        "    min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #27AE60;"
        "}"
        );

    // Add widgets to layout
    layout->addWidget(titleLabel);
    layout->addWidget(itemsTable);
    layout->addWidget(purchaseButton);
    layout->addWidget(backButton);

    // Connect buttons
    connect(backButton, &QPushButton::clicked, this, &MainWindow::returnToMain);
    connect(purchaseButton, &QPushButton::clicked, this, &MainWindow::handleItemPurchase);
}

void MainWindow::refreshTables() {
    // Refresh stock table
    QSqlQuery stockQuery("SELECT item_name, price, stock FROM stock_67011755");
    stockTable->setRowCount(0);
    while (stockQuery.next()) {
        int row = stockTable->rowCount();
        stockTable->insertRow(row);
        stockTable->setItem(row, 0, new QTableWidgetItem(stockQuery.value(0).toString()));
        stockTable->setItem(row, 1, new QTableWidgetItem(stockQuery.value(1).toString()));
        stockTable->setItem(row, 2, new QTableWidgetItem(stockQuery.value(2).toString()));
    }

    // Refresh change box table
    QSqlQuery changeQuery("SELECT THB, Count FROM change_box_67011755");
    changeBoxTable->setRowCount(0);
    while (changeQuery.next()) {
        int row = changeBoxTable->rowCount();
        changeBoxTable->insertRow(row);
        changeBoxTable->setItem(row, 0, new QTableWidgetItem(changeQuery.value(0).toString()));
        changeBoxTable->setItem(row, 1, new QTableWidgetItem(changeQuery.value(1).toString()));
    }

    // Refresh collection box table
    QSqlQuery collectionQuery("SELECT THB, Count FROM collection_box_67011755");
    collectionBoxTable->setRowCount(0);
    while (collectionQuery.next()) {
        int row = collectionBoxTable->rowCount();
        collectionBoxTable->insertRow(row);
        collectionBoxTable->setItem(row, 0, new QTableWidgetItem(collectionQuery.value(0).toString()));
        collectionBoxTable->setItem(row, 1, new QTableWidgetItem(collectionQuery.value(1).toString()));
    }

    // Refresh user items table
    QSqlQuery itemsQuery("SELECT item_name, price, stock FROM stock_67011755 WHERE stock > 0");
    itemsTable->setRowCount(0);
    while (itemsQuery.next()) {
        int row = itemsTable->rowCount();
        itemsTable->insertRow(row);
        itemsTable->setItem(row, 0, new QTableWidgetItem(itemsQuery.value(0).toString()));
        itemsTable->setItem(row, 1, new QTableWidgetItem(itemsQuery.value(1).toString()));
        itemsTable->setItem(row, 2, new QTableWidgetItem(itemsQuery.value(2).toString()));
    }
}

void MainWindow::showAdminMode() {
    stackedWidget->setCurrentWidget(adminPage);
    refreshTables();
}

void MainWindow::showUserMode() {
    if (!checkOperatingConditions()) {
        QMessageBox::warning(this, "System Status",
                             "Vending machine is currently not operational.\nPlease contact administrator.");
        return;
    }
    stackedWidget->setCurrentWidget(userPage);
    refreshTables();
}

void MainWindow::returnToMain() {
    stackedWidget->setCurrentWidget(mainPage);
}

void MainWindow::addNewItem() {
    bool ok;
    QString itemName = QInputDialog::getText(this, "Add New Item",
                                             "Enter item name:", QLineEdit::Normal, "", &ok);
    if (!ok || itemName.isEmpty()) return;

    int price = QInputDialog::getInt(this, "Add New Item",
                                     "Enter price (THB):", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int stock = QInputDialog::getInt(this, "Add New Item",
                                     "Enter initial stock:", 0, 0, 1000, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("INSERT INTO stock_67011755 (item_name, price, stock) VALUES (?, ?, ?)");
    query.addBindValue(itemName.toLower());
    query.addBindValue(price);
    query.addBindValue(stock);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Item added successfully!");
        refreshTables();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add item: " + query.lastError().text());
    }
}

bool MainWindow::checkOperatingConditions() {
    QSqlQuery stockQuery("SELECT COUNT(*) FROM stock_67011755 WHERE stock = 0");
    QSqlQuery totalItemsQuery("SELECT COUNT(*) FROM stock_67011755");
    QSqlQuery changeQuery("SELECT COUNT(*) FROM change_box_67011755 WHERE Count = 0");
    QSqlQuery collectionQuery("SELECT THB, Count FROM collection_box_67011755");

    stockQuery.next();
    totalItemsQuery.next();
    changeQuery.next();

    int outOfStock = stockQuery.value(0).toInt();
    int totalItems = totalItemsQuery.value(0).toInt();

    // Check if more than 50% items are out of stock
    bool lowStock = totalItems > 0 && (outOfStock >= totalItems / 2);

    // Check if change box has any zero count denominations
    bool insufficientChange = (changeQuery.value(0).toInt() > 0);

    // Check if collection box is full (max 100 for each denomination)
    bool collectionBoxFull = false;
    while (collectionQuery.next()) {
        if (collectionQuery.value(1).toInt() >= 100) {
            collectionBoxFull = true;
            break;
        }
    }

    return !(lowStock || insufficientChange || collectionBoxFull);
}

void MainWindow::deleteItem() {
    QModelIndexList selectedIndexes = stockTable->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Delete Item", "Please select an item to delete.");
        return;
    }

    QString itemName = stockTable->item(selectedIndexes.first().row(), 0)->text();
    QSqlQuery query;
    query.prepare("DELETE FROM stock_67011755 WHERE item_name = ?");
    query.addBindValue(itemName);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Item deleted successfully!");
        refreshTables();
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete item: " + query.lastError().text());
    }
}

void MainWindow::restockItem() {
    QModelIndexList selectedIndexes = stockTable->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Restock", "Please select an item to restock.");
        return;
    }

    QString itemName = stockTable->item(selectedIndexes.first().row(), 0)->text();
    bool ok;
    int amount = QInputDialog::getInt(this, "Restock Item",
                                      "Enter amount to add:", 0, 0, 1000, 1, &ok);
    if (!ok) return;

    QSqlQuery query;
    query.prepare("UPDATE stock_67011755 SET stock = stock + ? WHERE item_name = ?");
    query.addBindValue(amount);
    query.addBindValue(itemName);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Item restocked successfully!");
        refreshTables();
    } else {
        QMessageBox::critical(this, "Error", "Failed to restock item: " + query.lastError().text());
    }
}

void MainWindow::refillChange() {
    QStringList denominations = {"20THB", "10THB", "5THB", "1THB"};
    for (const QString &denom : denominations) {
        bool ok;
        int count = QInputDialog::getInt(this, "Refill Change",
                                         "Enter amount of " + denom + " to add:",
                                         0, 0, 1000, 1, &ok);
        if (!ok) continue;

        QSqlQuery query;
        query.prepare("UPDATE change_box_67011755 SET Count = Count + ? WHERE THB = ?");
        query.addBindValue(count);
        query.addBindValue(denom);

        if (!query.exec()) {
            QMessageBox::critical(this, "Error", "Failed to refill change: " + query.lastError().text());
            return;
        }
    }

    QMessageBox::information(this, "Success", "Change box refilled successfully!");
    refreshTables();
}

void MainWindow::collectMoney() {
    QSqlQuery query("UPDATE collection_box_67011755 SET Count = 0");

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Money collected successfully!");
        refreshTables();
    } else {
        QMessageBox::critical(this, "Error", "Failed to collect money: " + query.lastError().text());
    }
}

// Implement handleItemPurchase function
void MainWindow::handleItemPurchase() {
    QModelIndexList selectedIndexes = itemsTable->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Purchase", "Please select an item to purchase.");
        return;
    }

    int row = selectedIndexes.first().row();
    QString itemName = itemsTable->item(row, 0)->text();
    int price = itemsTable->item(row, 1)->text().toInt();
    int stock = itemsTable->item(row, 2)->text().toInt();

    if (stock <= 0) {
        QMessageBox::warning(this, "Out of Stock", "Selected item is out of stock.");
        return;
    }

    processPayment(itemName, price);
}


// Implement processPayment function
void MainWindow::processPayment(const QString &itemName, int price) {
    QStringList validDenominations = {"1", "5", "10", "20", "100"};
    int totalPayment = 0;
    QMap<int, int> paymentBreakdown;

    while (totalPayment < price) {
        bool ok;
        QString denomination = QInputDialog::getText(this, "Payment",
                                                     QString("Item Price: %1 THB\nRemaining to pay: %2 THB\n"
                                                             "Enter denomination (1,5,10,20,100 THB):")
                                                         .arg(price).arg(price - totalPayment),
                                                     QLineEdit::Normal, "", &ok);

        if (!ok) return;

        if (!validDenominations.contains(denomination)) {
            QMessageBox::warning(this, "Invalid Denomination",
                                 "Please use only 1, 5, 10, 20, or 100 THB denominations.");
            continue;
        }

        int denomValue = denomination.toInt();
        totalPayment += denomValue;
        paymentBreakdown[denomValue]++;
    }

    if (totalPayment < price) {
        QMessageBox::warning(this, "Insufficient Payment",
                             "Payment amount is less than the item price.");
        return;
    }

    int changeAmount = totalPayment - price;

    // Fetch current change box status
    QSqlQuery changeQuery("SELECT THB, Count FROM change_box_67011755");
    QMap<int, int> availableChange;
    while (changeQuery.next()) {
        QString denomStr = changeQuery.value(0).toString();
        int denom = denomStr.replace("THB", "").toInt();
        int count = changeQuery.value(1).toInt();
        availableChange[denom] = count;
    }

    // Calculate and update change
    QVector<int> denominations = {20, 10, 5, 1};
    QMap<int, int> changeBreakdown;

    for (int denom : denominations) {
        int availCount = availableChange[denom];
        int requiredCount = changeAmount / denom;
        int actualCount = qMin(requiredCount, availCount);

        if (actualCount > 0) {
            changeBreakdown[denom] = actualCount;
            changeAmount -= actualCount * denom;
        }
    }

    if (changeAmount > 0) {
        QMessageBox::warning(this, "Insufficient Change",
                             "Unable to provide exact change. Please contact an administrator.");
        return;
    }

    // Update change box
    QSqlQuery updateChangeQuery;
    for (auto it = changeBreakdown.begin(); it != changeBreakdown.end(); ++it) {
        updateChangeQuery.prepare("UPDATE change_box_67011755 SET Count = Count - ? WHERE THB = ?");
        updateChangeQuery.addBindValue(it.value());
        updateChangeQuery.addBindValue(QString("%1THB").arg(it.key()));
        updateChangeQuery.exec();
    }

    // Update collection box for each payment denomination
    QSqlQuery collectionQuery;
    for (auto it = paymentBreakdown.begin(); it != paymentBreakdown.end(); ++it) {
        collectionQuery.prepare("UPDATE collection_box_67011755 SET Count = Count + ? WHERE THB = ?");
        collectionQuery.addBindValue(it.value());
        collectionQuery.addBindValue(QString("%1THB").arg(it.key()));
        collectionQuery.exec();
    }

    // Update stock
    updateStock(itemName);

    // Show success message
    QString changeMsg = "Purchase successful.\n\nChange breakdown:\n";
    for (auto it = changeBreakdown.begin(); it != changeBreakdown.end(); ++it) {
        changeMsg += QString("%1 x %2 THB\n").arg(it.value()).arg(it.key());
    }
    QMessageBox::information(this, "Purchase Complete", changeMsg);

    // Refresh tables
    refreshTables();
}
// Implement calculateChange function (already mostly covered in processPayment)
void MainWindow::calculateChange(int changeAmount) {
    QVector<int> denominations = {20, 10, 5, 1};
    QMap<int, int> changeBreakdown;

    for (int denom : denominations) {
        int count = changeAmount / denom;
        if (count > 0) {
            changeBreakdown[denom] = count;
            changeAmount %= denom;
        }
    }

    // You can use changeBreakdown for further processing if needed
}

void MainWindow::updateStock(const QString &itemName) {
    QSqlQuery query;
    query.prepare("UPDATE stock_67011755 SET stock = stock - 1 WHERE item_name = ?");
    query.addBindValue(itemName);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update stock: " + query.lastError().text());
    }
}

MainWindow::~MainWindow() {
    // Clean up is handled automatically by Qt's parent-child system
}
