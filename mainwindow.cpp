#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regular_parking.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_regular_check_clicked()
{
    this->hide();
    regular_parking regular;
    regular.setModal(true);
    regular.exec();
    this->show();
}
