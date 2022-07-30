#include "sales.h"
#include "ui_sales.h"


sales::sales(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sales)
{
    ui->setupUi(this);
    // now 현재날짜로 설정
    now = QDate::currentDate();
    dailysales();
}

void sales::dailysales()
{

    QList<int> list;
    QList<QString> days;
    // 현재 날짜로부터 6일전 날짜를 start_day로 설정
    QDate start_day = now.addDays(-6);
    for(int i = 0; i<7; i++) // 날짜, 요금 리스트 만들기
    {
        // 날짜를 QString 으로 변환
        QString x = start_day.toString("yyyy-M-d");
        qDebug() << x;
        // QString list에 날짜 추가
        days.append(x);
        query = "SELECT sum(charge) from parking where date_out = '" + x.toStdString() + "'";
        sql.exec(QString::fromStdString(query));
        record = sql.record();
        sql.next();
        // sql 결과 sum(charge)값 int로 변환
        int date_sales = sql.value(0).toInt();
        // int형 QList에 요금 추가
        list.append(date_sales);
        // start_day 날짜 1일 후로 변경
        start_day = start_day.addDays(1);
    }
    // line chart
    series = new QLineSeries();
    // 첫번째 날짜의 요금
    int max = list[0];
    for(int i=0; i<7; i++)
    {
        // 날짜별 요금
        int total = list[i];
        // 요금 비교해서 total이 더 크면 max를 변경
        if(total > max)
        {
            max = total;
        }
        // line chart에 점 추가(i는 순서, total은 요금)
        series->append(i, total);
    }
    //차트 생성
    chart = new QChart();
    //범례 숨기기
    chart->legend()->hide();
    //차트에 line chart 추가
    chart->addSeries(series);
    // QStringList에 날짜 추가
    QStringList dates;
    dates << days[0] << days[1] << days[2] << days[3] << days[4] << days[5] << days[6];
    // x축 생성
    axisX = new QBarCategoryAxis();
    // x축 눈금에 날짜 추가
    axisX->append(dates);
    // 차트 아래에 x축 표시
    chart->addAxis(axisX, Qt::AlignBottom);
    // line chart에 x축 추가
    series->attachAxis(axisX);
    // x축 범위 설정
    axisX->setRange(days[0], days[6]);
    // y축 생성
    axisY = new QValueAxis();
    // 차트 왼쪽에 y축
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    // y축 범위 설정
    axisY->setRange(0, max);
    // QChartView에 chart추가
    chartView = new QChartView(chart);
    // chartView속성 설정
    chartView->setRenderHint(QPainter::Antialiasing);
    // layout에 chartview위젯 표시
    ui->verticalLayout->addWidget(chartView);

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(7);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("날짜"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("매출"));
    // 칸 가로 길이 동일하게 설정
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 칸 세로 길이 동일하게 설정
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for(int i=0; i<7; i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(days[i]));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(list[i])));
    }

}


sales::~sales()
{
    delete ui;
}

void sales::on_before_btn_clicked()
{
    // 7일전
    now = now.addDays(-7);
    // layout에서 chartView위젯 삭제
    ui->verticalLayout->removeWidget(chartView);
    dailysales();
}

void sales::on_next_btn_clicked()
{
    now = now.addDays(7);
    QDate current = QDate::currentDate();
    // 현재날짜와 now 일 수 차이
    int diff = current.daysTo(now);
    // 차이가 0보다 크면 => now 날짜가 current 날짜보다 미래
    if(diff>0)
    {
        now = QDate::currentDate();
        QMessageBox::information(this, "", "최근 매출입니다");
    }
    ui->verticalLayout->removeWidget(chartView);
    dailysales();
}

