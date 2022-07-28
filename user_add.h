#ifndef USER_ADD_H
#define USER_ADD_H
#include "database.h"
#include <QDialog>

namespace Ui {
class user_add;
}

class user_add : public QDialog
{
    Q_OBJECT

public:
    explicit user_add(QWidget *parent = nullptr);
    explicit user_add(std::vector<std::string> data, QWidget *parent = nullptr);
    ~user_add();

private slots:
    void on_add_btn_clicked();

    void on_edit_btn_clicked();

private:
    Ui::user_add *ui;
    std::string query;
    QSqlQuery sql;
    QSqlRecord record;
};

#endif // USER_ADD_H
