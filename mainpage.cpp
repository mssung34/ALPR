#include "mainpage.h"
#include "ui_mainpage.h"
#include "regist.h"
#include "cal.h"

mainpage::mainpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainpage)
{
    ui->setupUi(this);
    show_parking();
    QPixmap chart;
    int w = ui->label_5->width();
    int h = ui->label_5->height();
    chart.load("/home/iot/사진/car.png");
    ui->label_5->setPixmap(chart.scaled(w,h));  //

}

mainpage::~mainpage()
{
    delete ui;
}

void mainpage::show_parking()
{
    query_string="SELECT plate FROM current";
    query.exec(QString::fromStdString(query_string));
    query.next();
    record=query.record();
    int car=200-query.size();
    ui->label->setText("주차장 현황");
    ui->label_2->setText("주차가능 대수:"+QString::fromStdString(std::to_string(car))+"");
    ui->label_3->setText("총 주차공간:200");

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
    show_parking();
    this->show();
}
