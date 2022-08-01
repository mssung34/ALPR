#include "cal.h"
#include "ui_cal.h"

cal::cal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cal)
{
    ui->setupUi(this);
}

cal::~cal()
{
    delete ui;
}

void cal::point() // user 테이블에서 phone,point  select 하기
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


void cal::show_up() //insert 전  --> 입장 날짜,시간 출력
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

int cal::find() // 현재시간 과 입장시간을 비교해서 요금 계산 및 parking 테이블 에 저장
{
    QTime time= QTime::currentTime(); //현재시간
    QString now_time=time.toString("H:m"); // 현재시간을 insert하기위해 형변환
    QDate date = QDate::currentDate(); // 현재 날짜
    QString now_date=date.toString("yyyy-M-d"); // 현재날짜 insert하기위해 형변환
    QDateTime now_datetime=QDateTime::currentDateTime();

    query_string="SELECT * FROM current WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();

    QString past_date=query.value(1).toString();
    QString past_time=query.value(2).toString(); // select로 찾아온 입장시간을 string으로 저장
    QString past_datetime=past_date+" "+past_time;
    QDateTime Past_Time = QDateTime::fromString(past_datetime, "yyyy-M-d H:m");

    int sub_time=Past_Time.secsTo(now_datetime)/60;
    charge=sub_time * 10; // 분당 50원

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

void cal::show_down() // insert 후 --> 퇴장 날짜,시간 결제금액 출력
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

void cal::del() // 계산을 끝내면 current 테이블에서 해당 차량 삭제
{
    query_string="DELETE FROM current WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.first();
}

void cal::use_point(int charge) // charge를 받아서 point 사용 시 금액 계산
{
    query_string="SELECT point FROM user WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();
    record=query.record();
    int point=query.value(0).toInt();  //DB에 있는 포인트
    std::cout<<point<<std::endl;
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
    }

}

void cal::take_point(int charge) // 포인트적립
{
    query.prepare("INSERT INTO user (plate, phone)" "VALUES ( ?, ?)");
    query.addBindValue(ui->plate_input->text());
    query.addBindValue(ui->phone_input->text());
    query.exec();

    query_string="SELECT point FROM user WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();
    record=query.record();
    int point=query.value(0).toInt();  //DB에 있는 포인트

    int charge_c=charge;
    int take_point=charge_c / 10; // 10% 포인트
    int all_point=take_point+point; // 적립하고 난 후 포인트
    std::cout<<take_point<<std::endl;
    std::cout<<point<<std::endl;
    std::cout<<all_point<<std::endl;

    std::string tot_point=std::to_string(all_point); // 쿼리문 쓸려고 형변환
    query_string="UPDATE user SET point='"+ tot_point + "' WHERE phone='"+ui->phone_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));

}
void cal::on_find_btn_clicked() // 조회버튼 눌렀을때
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

void cal::on_cal_btn_clicked() // 계산버튼 눌렀을때
{

    query_string="SELECT regist_user FROM user WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();
    record=query.record();
    if(query.value(0)==1) // 정기주차 유저는 나중에 한번에 결제
    {
        QMessageBox::information(this,"정기주차자","정기주차자는 월말에 한번에 결제됩니다!");
        del();
        this->hide();

    }
    else // 일반유저는 계산완료 했다고 띄워줌
    {
        QMessageBox::information(this,"succse","계산 완료! 다음에 또 만나요~!");
        del();
        this->hide();

    }

}

void cal::on_point_btn_clicked() // 포인트 사용 버튼 클릭
{
    use_point(charge);
}

void cal::on_take_btn_clicked() // 포인트 적립 버튼 클릭
{
    query_string="SELECT plate FROM current WHERE plate='"+ui->plate_input->text().toStdString()+"'";
    query.exec(QString::fromStdString(query_string));
    query.next();
    record=query.record();
    if(query.value(0) !=ui->plate_input->text())
    {
        QMessageBox::information(this,"Error","차량 번호를 입력하세요");
    }
    else
    {
        take_point(charge);
        QMessageBox::information(this,"success","포인트 적립완료!");
    }
}
