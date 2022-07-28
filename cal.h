#ifndef CAL_H
#define CAL_H

#include <QDialog>

namespace Ui {
class cal;
}

class cal : public QDialog
{
    Q_OBJECT

public:
    explicit cal(QWidget *parent = nullptr);
    ~cal();

private:
    Ui::cal *ui;
};

#endif // CAL_H
