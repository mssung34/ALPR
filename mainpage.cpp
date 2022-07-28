#include "mainpage.h"
#include "ui_mainpage.h"
#include "regist.h"
#include "cal.h"

mainpage::mainpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainpage)
{
    ui->setupUi(this);

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


void mainpage::on_calcul_btn_clicked()
{
    this->hide();
    cal cal;
    cal.setModal(true);
    cal.exec();
    this->show();
}
