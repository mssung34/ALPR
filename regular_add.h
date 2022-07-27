#ifndef REGULAR_ADD_H
#define REGULAR_ADD_H
#include "database.h"
#include <QDialog>

namespace Ui {
class regular_add;
}

class regular_add : public QDialog
{
    Q_OBJECT

public:
    explicit regular_add(QWidget *parent = nullptr);
    explicit regular_add(std::vector<std::string> data, QWidget *parent = nullptr);
    ~regular_add();
    void show_date();
private slots:
    void on_start_date_selectionChanged();

    void on_end_date_selectionChanged();

    void on_register_btn_clicked();

    void on_exit_btn_clicked();

    void on_edit_btn_clicked();

private:
    Ui::regular_add *ui;
    std::string query;
    std::vector <std::string> datalist;
    QSqlQuery sql;
};

#endif // REGULAR_ADD_H
