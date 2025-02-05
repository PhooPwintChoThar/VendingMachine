
#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>

class Database {
public:
    static bool initialize() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("vending_machine.db");

        if (!db.open()) {
            qDebug() << "Error: connection with database failed";
            return false;
        }

        QSqlQuery query;

        // Create stock table
        if (!query.exec("CREATE TABLE IF NOT EXISTS stock_67011755("
                        "item_name TEXT NOT NULL,"
                        "price INTEGER NOT NULL,"
                        "stock INTEGER NOT NULL"
                        ");")) {
            qDebug() << "Error creating stock table:" << query.lastError();
            return false;
        }

        // Create change box table
        if (!query.exec("CREATE TABLE IF NOT EXISTS change_box_67011755("
                        "THB TEXT NOT NULL,"
                        "Count INTEGER NOT NULL"
                        ");")) {
            qDebug() << "Error creating change box table:" << query.lastError();
            return false;
        }

        // Create collection box table
        if (!query.exec("CREATE TABLE IF NOT EXISTS collection_box_67011755("
                        "THB TEXT NOT NULL,"
                        "Count INTEGER NOT NULL"
                        ");")) {
            qDebug() << "Error creating collection box table:" << query.lastError();
            return false;
        }

        return true;
    }

    static bool isTableEmpty(const QString& tableName) {
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM " + tableName);

        if (query.exec() && query.next()) {
            return query.value(0).toInt() == 0;
        }
        return true;
    }

    static bool initializeChangeBox() {
        if (!isTableEmpty("change_box_67011755")) {
            return true;
        }

        QSqlQuery query;
        QStringList denominations = {"20THB", "10THB", "5THB", "1THB"};

        for (const QString& denom : denominations) {
            query.prepare("INSERT OR REPLACE INTO change_box_67011755 (THB, Count) VALUES (?, 0)");
            query.addBindValue(denom);

            if (!query.exec()) {
                qDebug() << "Error initializing change box:" << query.lastError();
                return false;
            }
        }
        return true;
    }

    static bool initializeCollectionBox() {
        if (!isTableEmpty("collection_box_67011755")) {
            return true;
        }

        QSqlQuery query;
        QStringList denominations = {"100THB", "20THB", "10THB", "5THB", "1THB"};

        for (const QString& denom : denominations) {
            query.prepare("INSERT OR REPLACE INTO collection_box_67011755 (THB, Count) VALUES (?, 0)");
            query.addBindValue(denom);

            if (!query.exec()) {
                qDebug() << "Error initializing collection box:" << query.lastError();
                return false;
            }
        }
        return true;
    }
};

#endif // DATABASE_H
