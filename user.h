#ifndef USER_H
#define USER_H

#include <QDialog>
#include "database.h"

namespace Ui {
class user;
}

class user : public QDialog
{
    Q_OBJECT

public:
    explicit user(QWidget *parent = nullptr);
    ~user();
    void list();

private slots:
    void on_remove_btn_clicked();

    void on_edit_btn_clicked();

    void on_add_btn_clicked();

private:
    Ui::user *ui;
    std::string query;
    QSqlQuery sql;
    QSqlRecord record;
};

#endif // USER_H
