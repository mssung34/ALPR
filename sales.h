#ifndef SALES_H
#define SALES_H
#include "database.h"
#include <QDialog>
#include <QList>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartGlobal>
#include <QtCharts>
#include <QCloseEvent>

namespace Ui {
class sales;
}

class sales : public QDialog
{
    Q_OBJECT

public:
    explicit sales(QWidget *parent = nullptr);
    ~sales();
    void dailysales();


private slots:
    void on_before_btn_clicked();

    void on_next_btn_clicked();

private:
    Ui::sales *ui;
    std::string query;
    QSqlQuery sql;
    QSqlRecord record;
    QChartView *chartView;
    QLineSeries *series;
    QDate now;
    QChart *chart;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;

};

#endif // SALES_H
