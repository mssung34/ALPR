
#include "mainpage.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainpage main;
    main.show();
    return a.exec();
}
