#include "sales.h"
#include "ui_sales.h"


sales::sales(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sales)
{
    ui->setupUi(this);
    now = QDate::currentDate();
    dailysales();
}

void sales::dailysales()
{

    QList<int> list;
    QList<QString> days;
    // 현재 날짜로부터 6일전
    QDate start_day = now.addDays(-6);
    for(int i = 0; i<7; i++)
    {
        QString x = start_day.toString("yyyy-MM-dd");
        days.append(x);
        query = "SELECT sum(charge) from parking where date_out = '" + x.toStdString() + "'";
        sql.exec(QString::fromStdString(query));
        record = sql.record();
        sql.next();
        int date_sales = sql.value(0).toInt();
        list.append(date_sales);
        start_day = start_day.addDays(1);
    }
    // line chart
    series = new QLineSeries();
    int max = list[0];
    for(int i=0; i<7; i++)
    {
        int total = list[i];
        if(total > max)
        {
            max = total;
        }
        // line chart에 점 추가
        series->append(i, total);
    }

    chart = new QChart();
    //범례 숨기기
    chart->legend()->hide();
    chart->addSeries(series);
    QStringList dates;
    dates << days[0] << days[1] << days[2] << days[3] << days[4] << days[5] << days[6];
    // x축 눈금 설정
    axisX = new QBarCategoryAxis();
    axisX->append(dates);
    chart->addAxis(axisX, Qt::AlignBottom);
    // line chart에 x축 추가축
    series->attachAxis(axisX);
    // x축 범위 설정
    axisX->setRange(days[0], days[6]);
    // y축
    axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    axisY->setRange(0, max);
    // QChartView에 chart입력
    chartView = new QChartView(chart);
    // chartView속성 설정
    chartView->setRenderHint(QPainter::Antialiasing);
    // layout에 chartview 표시
    ui->verticalLayout->addWidget(chartView);

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(7);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("날짜"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("매출"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
    ui->verticalLayout->removeWidget(chartView);
    dailysales();
}

void sales::on_next_btn_clicked()
{
    now = now.addDays(7);
    QDate current = QDate::currentDate();
    int diff = current.daysTo(now);
    if(diff>0)
    {
        now = QDate::currentDate();
        QMessageBox::information(this, "", "최근");
    }
    ui->verticalLayout->removeWidget(chartView);
    dailysales();
}

