#include "mainpage.h"
#include "ui_mainpage.h"
#include "regist.h"

mainpage::mainpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainpage)
{
    ui->setupUi(this);
    show_time();
}

mainpage::~mainpage()
{
    delete ui;
}

void mainpage::on_end_btn_clicked()
{
    this->close();
}

void mainpage::on_regist_btn_clicked()
{
    this->hide();
    regist regist;
    regist.setModal(true);
    regist.exec();
    this->show();

}

void mainpage::show_time()
{
    QTime time = QTime::currentTime();
    QString time_text=time.toString();
    qDebug()<<time_text;
    //("hh: mm : ss")
    ui->start_time->setText(time_text);
}
