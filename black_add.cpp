#include "black_add.h"
#include "ui_black_add.h"
#include <QMessageBox>
black_add::black_add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::black_add)
{
    ui->setupUi(this);
}

black_add::~black_add()
{
    delete ui;
}

void black_add::on_add_btn_clicked()
{
    sql.prepare("INSERT INTO blacklist (plate)" "VALUES (?)");
    sql.addBindValue(ui->plate_txt->text());
    sql.exec();
    QMessageBox::information(this, "", "등록완료");
    this->close();
}
