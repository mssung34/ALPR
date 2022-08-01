#include "parking.h"
#include "ui_parking.h"
#include <QMessageBox>
parking::parking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::parking)
{
    ui->setupUi(this);
    ui->in_label->hide();
    parkinglist();
}



parking::parking(std::string data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::parking)
{
    ui->setupUi(this);
    ui->in_out_label->hide();
    currentlist();

}



void parking::currentlist()
{
    ui->tableWidget->clear();
    query = "SELECT plate, time_in, date_in FROM current";
    sql.exec(QString::fromStdString(query));
    record = sql.record();
    int plate = record.indexOf("plate");
    int date = record.indexOf("date_in");
    int in = record.indexOf("time_in");
    ui->tableWidget->setRowCount(sql.size());
    possible = 200 - sql.size();
    ui->tableWidget->setColumnCount(record.count());

    ui->label->setText("주차가능대수 : " + QString::fromStdString(std::to_string(possible)));

    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("차량번호"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("입차날짜"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("입차시간"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int i = 0;
    while(sql.next())
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(sql.value(plate).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(sql.value(date).toString()));
        ui->tableWidget->setItem(i++, 2, new QTableWidgetItem(sql.value(in).toString()));
    }
}


void parking::parkinglist()
{
    ui->tableWidget->clear();
    query = "SELECT num, plate, date_in, time_in, date_out, time_out, charge FROM parking";
    sql.exec(QString::fromStdString(query));
    record = sql.record();
    int num = record.indexOf("num");
    int plate = record.indexOf("plate");
    int time_in = record.indexOf("time_in");
    int time_out = record.indexOf("time_out");
    int date_in = record.indexOf("date_in");
    int date_out = record.indexOf("date_out");
    int charge = record.indexOf("charge");

    ui->tableWidget->setRowCount(sql.size());
    ui->tableWidget->setColumnCount(record.count());

    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("번호"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("차량번호"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("입차날짜"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("입차시간"));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("출차날짜"));
    ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("출차시간"));
    ui->tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("요금"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int i = 0;
    while(sql.next())
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(sql.value(num).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(sql.value(plate).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(sql.value(date_in).toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(sql.value(time_in).toString()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(sql.value(date_out).toString()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(sql.value(time_out).toString()));
        ui->tableWidget->setItem(i++, 6, new QTableWidgetItem(sql.value(charge).toString()));
    }

}

parking::~parking()
{
    delete ui;
}


