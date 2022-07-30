#ifndef PARKING_H
#define PARKING_H
#include "database.h"
#include <QDialog>

namespace Ui {
class parking;
}

class parking : public QDialog
{
    Q_OBJECT

public:
    explicit parking(QWidget *parent = nullptr);
    explicit parking(std::string data, QWidget *parent = nullptr);
    ~parking();
    void parkinglist();
    void currentlist();


private:
    Ui::parking *ui;
    std::string query;
    QSqlQuery sql;
    QSqlRecord record;
    int possible;
};

#endif // PARKING_H
