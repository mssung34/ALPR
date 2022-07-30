#ifndef BLACK_ADD_H
#define BLACK_ADD_H

#include <QDialog>
#include "database.h"

namespace Ui {
class black_add;
}

class black_add : public QDialog
{
    Q_OBJECT

public:
    explicit black_add(QWidget *parent = nullptr);
    ~black_add();

private slots:
    void on_add_btn_clicked();

private:
    Ui::black_add *ui;

    std::string query;
    QSqlQuery sql;
    QSqlRecord record;
};

#endif // BLACK_ADD_H
