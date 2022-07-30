#include "thread.h"
#include <QMessageBox>

Thread::Thread(QTcpSocket * clntsocket, QObject *parent) :
    QThread(parent)
{
    socket = clntsocket;
}

void Thread::run()
{
    qDebug() <<"start thread";
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    exec();

}

void Thread::readyRead()
{
    QString msg = QString(socket->readAll());
    msg = msg + " 입차";
    qDebug() << msg;
    QMessageBox box;
    box.setText(msg);
    box.setStandardButtons(QMessageBox::Ok);
    box.exec();
}


