#include "regular_parking.h"
#include "ui_regular_parking.h"
#include <QMessageBox>
#include "regular_add.h"

regular_parking::regular_parking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regular_parking)
{
    ui->setupUi(this);
    list();
}


void regular_parking::list()
{
    ui->tableWidget->clear();
    query = "SELECT plate, start, end, phone FROM regular";
    sql.exec(QString::fromStdString(query));
    record = sql.record();
    int plate = record.indexOf("plate"); // DB 컬럼명
    int start = record.indexOf("start");
    int end = record.indexOf("end");
    int phone = record.indexOf("phone");
    ui->tableWidget->setRowCount(sql.size());
    ui->tableWidget->setColumnCount(record.count());
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("차량번호"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("정기주차 신청날짜"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("정기주차 종료날짜"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("전화번호"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int i =0;
    while(sql.next())
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(sql.value(plate).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(sql.value(start).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(sql.value(end).toString()));
        ui->tableWidget->setItem(i++, 3, new QTableWidgetItem(sql.value(phone).toString()));

    }
}


regular_parking::~regular_parking()
{
    delete ui;
}

void regular_parking::on_add_btn_clicked()
{
    regular_add add;
    add.setModal(true);
    add.exec();
    list();
}

void regular_parking::on_edit_btn_clicked()
{
    if(ui->tableWidget->currentItem() == NULL)
    {
        QMessageBox::information(this, "", "선택하세요");
    }
    else
    {
        int row;
        std::vector<std::string>data;

        row = ui->tableWidget->currentRow();
        data.push_back(ui->tableWidget->takeItem(row, 0)->text().toStdString());
        data.push_back(ui->tableWidget->takeItem(row, 1)->text().toStdString());
        data.push_back(ui->tableWidget->takeItem(row, 2)->text().toStdString());
        data.push_back(ui->tableWidget->takeItem(row, 3)->text().toStdString());
        regular_add edit(data);
        edit.setModal(true);
        edit.exec();
        list();
    }
}

void regular_parking::on_remove_btn_clicked()
{
    if(ui->tableWidget->currentItem() == NULL)
    {
        QMessageBox::information(this, "", "선택하세요");
    }
    else
    {
        QMessageBox box;
        box.setText("삭제하시겠습니까?");
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        box.setDefaultButton(QMessageBox::Yes);
        if(box.exec() == QMessageBox::Yes)
        {
            int row = ui->tableWidget->currentRow();
            query = "DELETE FROM regular WHERE plate = '" + ui->tableWidget->takeItem(row, 0)->text().toStdString() + "'";
            sql.exec(QString::fromStdString(query));
            QMessageBox::information(this, "", "삭제완료");
            list();
        }
    }
}
