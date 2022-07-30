#ifndef BLACKLIST_H
#define BLACKLIST_H
#include "database.h"
#include <QDialog>

namespace Ui {
class blacklist;
}

class blacklist : public QDialog
{
    Q_OBJECT

public:
    explicit blacklist(QWidget *parent = nullptr);
    ~blacklist();

private slots:
    void on_add_btn_clicked();

    void on_remove_btn_clicked();
    void list();

private:
    Ui::blacklist *ui;

    std::string query;
    QSqlQuery sql;
    QSqlRecord record;
};

#endif // BLACKLIST_H
