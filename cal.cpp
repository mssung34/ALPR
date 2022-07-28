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
