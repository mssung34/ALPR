#include "sales.h"
#include "ui_sales.h"
#include <QList>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartGlobal>
#include <QtCharts>

sales::sales(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sales)
{
    ui->setupUi(this);
    dailysales();
}

void sales::dailysales()
{
    QDate now = QDate::currentDate();
    QList<int> list;
    QList<QString> days;
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

    QLineSeries *series = new QLineSeries();
    int max = list[0];
    for(int i=0; i<7; i++)
    {
        int total = list[i];
        if(total > max)
        {
            max = total;
        }
        series->append(i, total);
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    QStringList dates;
    dates << days[0] << days[1] << days[2] << days[3] << days[4] << days[5] << days[6];
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(dates);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisX->setRange(days[0], days[6]);
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    axisY->setRange(0, max);
    // QChartView에 chart입력
    QChartView *chartView = new QChartView(chart);
    // chartView속성 설정
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout->addWidget(chartView);
}

sales::~sales()
{
    delete ui;
}
