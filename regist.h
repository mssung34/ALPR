#ifndef REGULAR_ADD_H
#define REGULAR_ADD_H
#include "database.h"
#include "mainpage.h"
#include <QDialog>

namespace Ui {
class regist;
}

class regist : public QDialog
{
    Q_OBJECT

public:
    explicit regist(QWidget *parent = nullptr);
    explicit regist(std::vector<std::string> data, QWidget *parent = nullptr);
    ~regist();
    void show_date();
private slots:
    void on_start_date_selectionChanged();

    void on_end_date_selectionChanged();

    void on_register_btn_clicked();

    void on_exit_btn_clicked();

    void on_edit_btn_clicked();

private:
    Ui::regist *ui;
    std::string query;
    std::vector <std::string> datalist;
    QSqlQuery sql;
};

#endif // REGULAR_ADD_H
