#include "user_add.h"
#include "ui_user_add.h"
#include "database.h"
#include <QMessageBox>

user_add::user_add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user_add)
{
    ui->setupUi(this);
    ui->edit_btn->hide();
}

user_add::~user_add()
{
    delete ui;
}


user_add::user_add(std::vector<std::string> data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user_add)
{
    ui->setupUi(this);
    ui->add_btn->hide();
    ui->plate_txt->setText(QString::fromStdString(data[0]));
    ui->phone_txt->setText(QString::fromStdString(data[1]));
    if(data[2] == "O")
    {
        ui->apply_btn->setChecked(true);
    }
    else
    {
        ui->not_apply_btn->setChecked(true);
    }
    ui->point_txt->setText(QString::fromStdString(data[3]));

}

void user_add::on_add_btn_clicked()
{
    sql.prepare("INSERT INTO user (plate, phone, regist_user, point)" "VALUES (?, ?, ?, ?)");
    sql.addBindValue(ui->plate_txt->text());
    sql.addBindValue(ui->phone_txt->text());
    if(ui->apply_btn->isChecked())
    {
        sql.addBindValue("1");
    }
    else
    {
        sql.addBindValue("0");
    }
    sql.addBindValue(ui->point_txt->text());
    sql.exec();
    QMessageBox::information(this, "", "등록완료");
    this->close();
}

void user_add::on_edit_btn_clicked()
{

    sql.prepare("UPDATE user SET phone = ?, regist_user = ?, point = ? WHERE plate = ?");
    sql.addBindValue(ui->phone_txt->text());
    if(ui->apply_btn->isChecked())
    {
        sql.addBindValue("1");
    }
    else
    {
        sql.addBindValue("0");
    }
    sql.addBindValue(ui->point_txt->text());
    sql.addBindValue(ui->plate_txt->text());
    sql.exec();
    this->close();
}







