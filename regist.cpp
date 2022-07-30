#include "regist.h"
#include "ui_regist.h"
#include <QMessageBox>
#include "mainpage.h"
regist::regist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regist)
{
    ui->setupUi(this);
    ui->edit_btn->hide();
    show_date();
    QTime time = QTime::currentTime();
    QString time_text=time.toString();
    qDebug()<<time_text;

}

regist::regist(std::vector<std::string> data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regist)
{
    ui->setupUi(this);
    ui->register_btn->hide();
    ui->plate_txt->setText(QString::fromStdString(data[0]));
    ui->start_txt->setText(QString::fromStdString(data[1]));
    ui->end_txt->setText(QString::fromStdString(data[2]));
    QDate start = QDate::fromString(ui->start_txt->text(), "yyyy-M-d");
    QDate end = QDate::fromString(ui->end_txt->text(), "yyyy-M-d");
    ui->start_date->setSelectedDate(QDate(start));
    ui->end_date->setSelectedDate(QDate(end));

}

regist::~regist()
{
    delete ui;
}

void regist::show_date()
{
    QString now = QDate::currentDate().toString("yyyy-M-d");
    ui->start_txt->setText(now);
    ui->end_txt->setText(now);
}

void regist::on_start_date_selectionChanged()
{
    QString change = ui->start_date->selectedDate().toString("yyyy-M-d");
    QDate now = QDate::currentDate();
    int days = now.daysTo(ui->start_date->selectedDate());
    if(days<0)
    {
        QMessageBox::information(this, "", "선택불가능한 날짜입니다");

    }
    else
    {
        ui->start_txt->setText(change);
    }
}

void regist::on_end_date_selectionChanged()
{
    QString change = ui->end_date->selectedDate().toString("yyyy-M-d");
    int days = ui->start_date->selectedDate().daysTo(ui->end_date->selectedDate());
    if(days<0)
    {
        QMessageBox::information(this, "", "신청일보다 앞선 날짜입니다.");
    }
    {
        ui->end_txt->setText(change);
    }
}

void regist::on_register_btn_clicked()
{
    int days = ui->start_date->selectedDate().daysTo(ui->end_date->selectedDate());
    int point=0;
    std::string regist_point=std::to_string(point);

    if(ui->plate_txt->text().toStdString() == "")
    {
        QMessageBox::information(this, "", "차량번호를 입력하세요");
    }
    else if(ui->phone_txt->text().toStdString() == "")
    {
        QMessageBox::information(this,"","핸드폰 뒷자리를 입력하세요");
    }
    else if(days<0)
    {
        QMessageBox::information(this, "", "신청일보다 앞선 날짜입니다.");
    }
    else
    {
        query.prepare("INSERT INTO regular (plate, start, end, phone)" "VALUES (?, ?, ?, ?)");
        query.addBindValue(ui->plate_txt->text());
        query.addBindValue(ui->start_txt->text());
        query.addBindValue(ui->end_txt->text());
        query.addBindValue(ui->phone_txt->text());
        query.exec();

        query.prepare("INSERT INTO user (plate, phone, regist_user, point)" "VALUES (?, ?, ?, ?)");
        query.addBindValue(ui->plate_txt->text());
        query.addBindValue(ui->phone_txt->text());
        query.addBindValue("1");
        query.addBindValue("0");
        query.exec();

        query.first();
        query_string="UPDATE user SET regist_user = '1' WHERE plate='"+ui->plate_txt->text().toStdString()+"'";
        query.exec();
        QMessageBox::information(this, "", "추가완료");
        this->close();

    }
}

void regist::on_exit_btn_clicked()
{
    this->close();
}

