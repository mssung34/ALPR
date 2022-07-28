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

    void on_calcul_btn_clicked();

private:
    Ui::mainpage *ui;
    Database db;
    std::string query_string;
    QSqlQuery query;
    QSqlRecord record;

};

#endif // MAINPAGE_H
