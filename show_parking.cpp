#include "show_parking.h"
#include "ui_show_parking.h"

show_parking::show_parking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::show_parking)
{
    ui->setupUi(this);
}

show_parking::~show_parking()
{
    delete ui;
}
