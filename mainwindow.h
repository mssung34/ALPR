#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "database.h"
#
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_regular_check_clicked();

    void on_user_check_clicked();

    void on_parking_check_clicked();

    void on_current_check_clicked();

    void on_sales_check_clicked();

private:
    Ui::MainWindow *ui;
    Database DB;

};
#endif // MAINWINDOW_H
