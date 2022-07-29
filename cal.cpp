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
        ui->label_2->setText("입장 날짜: "+query.value(date_in).toString()+"\n");
        ui->label_3->setText("입장 시간: "+query.value(time_in).toString()+"\n");
    }
}

int cal::find()
{
    QTime time= QTime::currentTime(); //현재시간
    QString now_time=time.toString("HH:mm"); // 현재시간을 insert하기위해 형변환
    QDate date = QDate::currentDate(); // 현재 날짜
    QString now_date=date.toString("yyyy-MM-dd"); // 현재날짜 insert하기위해 형변환
    int Time_M=time.minute(); // 현재시간 분
    int Time_H=time.hour(); // 현재시간 시간
    int cal_Time=Time_H*60 + Time_M; // 총 분 으로 바꿈 (현재시간)

    query_string="SELECT * FROM current WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();

    QString cur_time=query.value(2).toString(); // select로 찾아온 입장시간을 string으로 저장
    QTime time2=time2.fromString(cur_time,"HH:mm"); // 입장시간을 QTime으로 저장
    int Time2_M=time2.minute(); // 입장시간 int
    int Time2_H=time2.hour(); // 입장시간 int
    int cal_Time2=Time2_H*60 + Time2_M; // 총 분 으로 바꿈 (입장시간)
    int all_time=cal_Time - cal_Time2; // 현재시간 - 입장시간  을 min 형태로
    charge=all_time * 50; // 분당 50원

    query2.prepare("INSERT INTO parking (plate,date_in,time_in,date_out,time_out,charge)" "VALUES (?,?,?,?,?,?)"); // 가격 계산해서 넣어야함
    query2.addBindValue(query.value(0));
    query2.addBindValue(query.value(1));
    query2.addBindValue(query.value(2));
    query2.addBindValue(now_date);
    query2.addBindValue(now_time);
    query2.addBindValue(charge);
    query2.exec();

    return charge;

}

void cal::show_down() // insert 후
{
    query_string="SELECT date_out,time_out,charge FROM parking WHERE plate='"+ui->plate_input->text().toStdString()+"'";
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

void cal::del()
{
    query_string="DELETE FROM current WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.first();
}

int cal::use_point(int charge)
{
    query_string="SELECT point FROM user WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();
    record=query.record();
    int point=query.value(0).toInt();  //DB에 있는 포인트
    if(ui->point_input->text().toInt() > point)
    {
        QMessageBox::information(this, "", "보유 포인트를 확인해주세요");
    }
    else
    {
        int point2=ui->point_input->text().toInt(); // 입력한 포인트
        charge-=point2; // 포인트를 계산한 총 금액
        std::string cal=std::to_string(charge); // 쿼리문 쓸려고 형변환
        query_string="UPDATE parking SET charge='"+ cal + "' WHERE plate='"+ui->plate_input->text().toStdString()+"'";
        query.exec(QString::fromStdString(query_string));

        int remain_point=point-point2; // 남은 포인트
        std::string re_point=std::to_string(remain_point); // 쿼리문 쓸려고 형변환
        query_string="UPDATE user SET point='"+ re_point + "' WHERE plate='"+ui->plate_input->text().toStdString()+"'";
        query.exec(QString::fromStdString(query_string));

        query_string="SELECT charge FROM parking WHERE plate='"+ui->plate_input->text().toStdString()+"'";
        query2.exec(QString::fromStdString(query_string));
        query2.next();
        record=query2.record();
        int charge_c=record.indexOf("charge");
        ui->label_6->clear();
        ui->label_6->setText("결제 금액: "+query2.value(charge_c).toString()+"\n");

        query_string="SELECT point FROM user WHERE plate='"+ui->plate_input->text().toStdString()+"'";
        query2.exec(QString::fromStdString(query_string));
        query2.next();
        record=query2.record();
        int point_c=record.indexOf("point");
        ui->label_7->clear();
        ui->label_7->setText("보유 포인트: "+query2.value(point_c).toString()+"\n");
        return remain_point;
    }

}

void cal::take_point(int charge,int remain_point) // 포인트적립
{
    int take_point=charge*(0.1); // 10% 포인트
    int all_point=take_point+remain_point; // 적립하고 난 후 포인트
    query.prepare("INSERT INTO user (plate, phone)" "VALUES ( ?, ?)");
    query.addBindValue(ui->plate_input->text());
    query.addBindValue(ui->phone_input->text());
    query.exec();

    std::string tot_point=std::to_string(all_point); // 쿼리문 쓸려고 형변환
    query_string="UPDATE user SET point='"+ tot_point + "' WHERE phone='"+ui->phone_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));

}
void cal::on_find_btn_clicked()
{
    query_string="SELECT plate FROM current WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();
    record=query.record();
    if(query.value(0) !=ui->plate_input->text())
    {
        QMessageBox::information(this,"Error","차량 번호를 확인해 주세요");
    }
    else
    {
        show_up();
        find();
        show_down();
    }
}

void cal::on_out_btn_clicked()
{
    this->hide();
}

void cal::on_cal_btn_clicked()
{

    query_string="SELECT regist_user FROM user WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();
    record=query.record();
    if(query.value(0)==1)
    {
        QMessageBox::information(this,"정기주차자","정기주차자는 월말에 한번에 결제됩니다!");
        del();
        this->hide();

    }
    else
    {
        QMessageBox::information(this,"succse","계산 완료! 다음에 또 만나요~!");
        del();
        this->hide();

    }

}

void cal::on_point_btn_clicked()
{
    use_point(charge);
}

void cal::on_take_btn_clicked()
{
    take_point(charge,remain_point);
    QMessageBox::information(this,"success","포인트 적립완료!");
}
