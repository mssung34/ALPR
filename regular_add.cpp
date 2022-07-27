#include "regular_add.h"
#include "ui_regular_add.h"
#include <QMessageBox>
regular_add::regular_add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regular_add)
{
    ui->setupUi(this);
    ui->edit_btn->hide();
    show_date();

}

regular_add::regular_add(std::vector<std::string> data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::regular_add)
{
    ui->setupUi(this);
    ui->register_btn->hide();
    ui->plate_txt->setText(QString::fromStdString(data[0]));
    ui->start_txt->setText(QString::fromStdString(data[1]));
    ui->end_txt->setText(QString::fromStdString(data[2]));
    //std::string startday = data[1];
    //std::replace(startday.begin(), startday.end(), '-', ',');
    QDate start = QDate::fromString(ui->start_txt->text(), "yyyy-MM-dd");
    QDate end = QDate::fromString(ui->end_txt->text(), "yyyy-MM-dd");
    ui->start_date->setSelectedDate(QDate(start));
    ui->end_date->setSelectedDate(QDate(end));

}

regular_add::~regular_add()
{
    delete ui;
}

void regular_add::show_date()
{
    QString now = QDate::currentDate().toString("yyyy-MM-dd");
    ui->start_txt->setText(now);
    ui->end_txt->setText(now);
}

void regular_add::on_start_date_selectionChanged()
{
    QString change = ui->start_date->selectedDate().toString("yyyy-MM-dd");
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

void regular_add::on_end_date_selectionChanged()
{
    QString change = ui->end_date->selectedDate().toString("yyyy-MM-dd");
    int days = ui->start_date->selectedDate().daysTo(ui->end_date->selectedDate());
    if(days<0)
    {
        QMessageBox::information(this, "", "신청일보다 앞선 날짜입니다.");
    }
    {
        ui->end_txt->setText(change);
    }
}

void regular_add::on_register_btn_clicked()
{
    int days = ui->start_date->selectedDate().daysTo(ui->end_date->selectedDate());

    if(ui->plate_txt->text().toStdString() == "")
    {
        QMessageBox::information(this, "", "차량번호를 입력하세요");
    }
    else if(days<0)
    {
        QMessageBox::information(this, "", "신청일보다 앞선 날짜입니다.");
    }
    else
    {
        sql.prepare("INSERT INTO regular (plate, start, end)" "VALUES (?, ?, ?)");
        sql.addBindValue(ui->plate_txt->text());
        sql.addBindValue(ui->start_txt->text());
        sql.addBindValue(ui->end_txt->text());
        sql.exec();
        QMessageBox::information(this, "", "추가완료");
        this->close();

    }
}

void regular_add::on_exit_btn_clicked()
{
    this->close();
}

void regular_add::on_edit_btn_clicked()
{
    query = "UPDATE regular SET start = '" + ui->start_txt->text().toStdString() + "', end ='" + ui->end_txt->text().toStdString() + "' where plate = '" + ui->plate_txt->text().toStdString() +"'";
    qDebug()<<QString::fromStdString(query);
    sql.exec(QString::fromStdString(query));
    QMessageBox::information(this, "", "수정완료");
    this->close();
}
