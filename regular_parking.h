#ifndef REGULAR_PARKING_H
#define REGULAR_PARKING_H

#include <QDialog>
#include "database.h"

namespace Ui {
class regular_parking;
}

class regular_parking : public QDialog
{
    Q_OBJECT

public:
    explicit regular_parking(QWidget *parent = nullptr);
    ~regular_parking();
    void list();

private slots:
    void on_add_btn_clicked();

    void on_edit_btn_clicked();

    void on_remove_btn_clicked();

private:
    Ui::regular_parking *ui;

    std::string query;
    QSqlQuery sql;
    QSqlRecord record;
};

#endif // REGULAR_PARKING_H
