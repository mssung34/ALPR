#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QDialog>
#include <QMainWindow>
#include <database.h>
#include <iostream>
#include <cstring>
#include <string>
#include <QMessageBox>
#include <QTimer>
#include "regist.h"


namespace Ui {
class mainpage;
}

class mainpage : public QDialog
{
    Q_OBJECT

public:
    explicit mainpage(QWidget *parent = nullptr);
    ~mainpage();

private slots:
    void on_end_btn_clicked();

    void on_regist_btn_clicked();

    void show_time();

private:
    Ui::mainpage *ui;
    Database db;
    QSqlQuery query;
    QSqlRecord record;
    std::string query_string;
};

#endif // MAINPAGE_H
