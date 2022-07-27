#ifndef SHOW_PARKING_H
#define SHOW_PARKING_H

#include <QDialog>

namespace Ui {
class show_parking;
}

class show_parking : public QDialog
{
    Q_OBJECT

public:
    explicit show_parking(QWidget *parent = nullptr);
    ~show_parking();

private:
    Ui::show_parking *ui;
};

#endif // SHOW_PARKING_H
