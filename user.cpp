#include "user.h"
#include "ui_user.h"
#include <QMessageBox>
#include "user_add.h"
user::user(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user)
{
    ui->setupUi(this);
    list();
}

void user::list()
{
    ui->tableWidget->clear();
    query = "SELECT plate, phone, regist_user, point FROM user";
    sql.exec(QString::fromStdString(query));
    record = sql.record();
    int plate = record.indexOf("plate");
    int phone = record.indexOf("phone");
    int regist_user = record.indexOf("regist_user");
    int point = record.indexOf("point");
    ui->tableWidget->setRowCount(sql.size());
    ui->tableWidget->setColumnCount(record.count());
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("차량번호"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("전화번호"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("정기주차"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("마일리지"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int i = 0;
    while(sql.next())
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(sql.value(plate).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(sql.value(phone).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(sql.value(regist_user).toString()));
        ui->tableWidget->setItem(i++, 3, new QTableWidgetItem(sql.value(point).toString()));
    }
}

user::~user()
{
    delete ui;
}

void user::on_remove_btn_clicked()
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
            query = "DELETE FROM user WHERE plate ='" + ui->tableWidget->takeItem(row, 0)->text().toStdString() + "'";
            sql.exec(QString::fromStdString(query));
            QMessageBox::information(this, "", "삭제완료");
            list();
        }

    }
}

void user::on_edit_btn_clicked()
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
        user_add edit(data);
        edit.setModal(true);
        edit.exec();
        list();
    }
}

void user::on_add_btn_clicked()
{
    //this->hide();
    user_add add;
    add.setModal(true);
    add.exec();
    //this->show();
    list();
}
