#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regular_parking.h"
#include "user.h"
#include "parking.h"
#include "sales.h"
#include "blacklist.h"
#include <QImage>
#include <QPixmap>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->move(750,250);
    socket = new QTcpSocket(this);
    socket->connectToHost("10.10.20.231", 9022);
    thread = new Thread(socket, this);
    thread->start();

    QPixmap pix("/home/iot/ALPR/car.png");
    ui->label->setPixmap(pix);



}

void MainWindow::closeEvent(QCloseEvent*)
{
    socket->close();
    thread->exit(0);
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

void MainWindow::on_user_check_clicked()
{
    this->hide();
    user user;
    user.setModal(true);
    user.exec();
    this->show();

}

void MainWindow::on_parking_check_clicked()
{
    this->hide();
    parking parking;
    parking.setModal(true);
    parking.exec();
    this->show();
}

void MainWindow::on_current_check_clicked()
{
    this->hide();
    std::string data = "current";
    parking current(data);
    current.setModal(true);
    current.exec();
    this->show();
}

void MainWindow::on_sales_check_clicked()
{
    this->hide();
    sales sales;
    sales.setModal(true);
    sales.exec();
    this->show();
}

void MainWindow::on_black_check_clicked()
{
    this->hide();
    blacklist black;
    black.setModal(true);
    black.exec();
    this->show();

}
