#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "database.h"
#include <QTcpSocket>
#include <QMainWindow>
#include "thread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent*);
    ~MainWindow();

private slots:
    void on_regular_check_clicked();

    void on_user_check_clicked();

    void on_parking_check_clicked();

    void on_current_check_clicked();

    void on_sales_check_clicked();



    void on_black_check_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    Database DB;
    Thread *thread;

};
#endif // MAINWINDOW_H
