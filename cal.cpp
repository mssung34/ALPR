#include "cal.h"
#include "ui_cal.h"

cal::cal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cal)
{
    ui->setupUi(this);
    QPixmap chart;
    int w = ui->chart->width();
    int h = ui->chart->height();
    chart.load("/home/iot/사진/chart.png");
    ui->chart->setPixmap(chart.scaled(w,h));
}

cal::~cal()
{
    delete ui;
}

void cal::show_up() //insert 전
{
    point();
    query_string=query_string="SELECT * FROM current WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    record=query.record();
    int date_in=record.indexOf("date_in");
    int time_in=record.indexOf("time_in");

    while(query.next())
    {
        //ui->label->setText("차량 번호: "+query.value(plate).toString()+"\n");
        ui->label_2->setText("입장 날짜: "+query.value(date_in).toString()+"\n");
        ui->label_3->setText("입장 시간: "+query.value(time_in).toString()+"\n");
    }
}

void cal::show_down() // insert 후
{
    query_string=query_string="SELECT date_out,time_out,charge FROM parking WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    record=query.record();
    int date_out=record.indexOf("date_out");
    int time_out=record.indexOf("time_out");
    int charge=record.indexOf("charge");

    while(query.next())
    {
        ui->label_4->setText("퇴장 날짜: "+query.value(date_out).toString()+"\n");
        ui->label_5->setText("퇴장 시간: "+query.value(time_out).toString()+"\n");
        ui->label_6->setText("결제 금액: "+query.value(charge).toString()+"\n");
    }
}

void cal::point()
{
    query_string=query_string="SELECT phone,point FROM user WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    record=query.record();
    int phone=record.indexOf("phone");
    int point=record.indexOf("point");
    while(query.next())
    {
        ui->label->setText("핸드폰 번호: "+query.value(phone).toString()+"\n");
        ui->label_7->setText("보유 포인트: "+query.value(point).toString()+"\n");

    }
}

void cal::find()
{
    QTime time= QTime::currentTime();
    QString now_time=time.toString("HH:mm");
    QDate date = QDate::currentDate();
    QString now_date=date.toString("yyyy-MM-dd");

    query_string=query_string="SELECT * FROM current WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();

    query2.prepare("INSERT INTO parking (plate,date_in,time_in,date_out,time_out)" "VALUES (?,?,?,?,?)"); // 가격 계산해서 넣어야함
    query2.addBindValue(query.value(0));
    query2.addBindValue(query.value(1));
    query2.addBindValue(query.value(2));
    query2.addBindValue(now_date);
    query2.addBindValue(now_time);
    //query.addBindValue("0");
    query2.exec();

}

void cal::del()
{
    query_string="DELETE FROM current WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.first();
}

void cal::on_find_btn_clicked()
{
   show_up();
   find();
   show_down();

}

void cal::on_out_btn_clicked()
{
    this->hide();
}

void cal::on_cal_btn_clicked()
{
    QMessageBox::information(this,"succse","계산 완료! 다음에 또 만나요~!");
    del();
    this->hide();
}

void cal::on_radioButton_clicked()
{
    slt==1;
}
