#include "blacklist.h"
#include "ui_blacklist.h"
#include <QMessageBox>
#include "black_add.h"

blacklist::blacklist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::blacklist)
{
    ui->setupUi(this);
    list();
}

blacklist::~blacklist()
{
    delete ui;
}

void blacklist::list()
{
    ui->tableWidget->clear();
    query = "SELECT plate FROM blacklist";
    sql.exec(QString::fromStdString(query));
    record = sql.record();
    int plate = record.indexOf("plate");
    ui->tableWidget->setRowCount(sql.size());
    ui->tableWidget->setColumnCount(record.count());
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("차량번호"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int i =0;
    while(sql.next())
    {
        ui->tableWidget->setItem(i++, 0, new QTableWidgetItem(sql.value(plate).toString()));
    }
}
void blacklist::on_add_btn_clicked()
{
    //this->hide();
    black_add add;
    add.setModal(true);
    add.exec();
    //this->show();
    list();
}

void blacklist::on_remove_btn_clicked()
{
    if(ui->tableWidget->currentItem() == NULL)
    {
        QMessageBox::information(this, "", "선택하세요");
    }
    else
    {
        QMessageBox box;
        box.setText("삭제하시겠습니까?");
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        box.setDefaultButton(QMessageBox::Yes);
        if(box.exec() == QMessageBox::Yes)
        {
            int row = ui->tableWidget->currentRow();
            query = "DELETE FROM blacklist WHERE plate = '" + ui->tableWidget->takeItem(row, 0)->text().toStdString() + "'";
            sql.exec(QString::fromStdString(query));
            QMessageBox::information(this, "", "삭제완료");
            list();
        }
    }
}
