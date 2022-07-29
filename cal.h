#ifndef CAL_H
#define CAL_H

#include <QDialog>
#include "mainpage.h"
#include <iostream>

namespace Ui {
class cal;
}

class cal : public QDialog
{
    Q_OBJECT

public:
    explicit cal(QWidget *parent = nullptr);
    void show_up();
    void show_down();
    void point();
    int find();
    void del();
    int use_point(int charge);
    void take_point(int charge,int remain_point);
    ~cal();

private slots:
    void on_find_btn_clicked();

    void on_out_btn_clicked();

    void on_cal_btn_clicked();

    void on_point_btn_clicked();

    void on_take_btn_clicked();

private:
    Ui::cal *ui;
    std::string query_string;
    QSqlQuery query;
    QSqlQuery query2;
    QSqlRecord record;
    int charge;
    int remain_point;

};

#endif // CAL_H
