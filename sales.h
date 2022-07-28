#ifndef SALES_H
#define SALES_H
#include "database.h"
#include <QDialog>

namespace Ui {
class sales;
}

class sales : public QDialog
{
    Q_OBJECT

public:
    explicit sales(QWidget *parent = nullptr);
    ~sales();
    void dailysales();

private:
    Ui::sales *ui;
    std::string query;
    QSqlQuery sql;
    QSqlRecord record;

};

#endif // SALES_H
