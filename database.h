#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QtSql>
#include <QSqlTableModel>

class Database
{
private:
    QSqlDatabase db;
public:
    Database() {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("10.10.20.231");      // IP 또는 DNS Host name
        db.setDatabaseName("alpr"); // DB명
        db.setUserName("admin");     // 계정 명
        db.setPassword("1q2w3e4r");     // 계정 Password

        if(!db.open())
        {
            qDebug()<<"connect error";
            exit(1);
        }
    }
    ~Database() {
        db.close();
    }
};

#endif // DATABASE_H
