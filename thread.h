#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include "database.h"

class Thread : public QThread
{
    Q_OBJECT

public:
    explicit Thread(QTcpSocket * clntsocket, QObject * parent=0);
private:
    void run();
    QTcpSocket * socket;

signals:

public slots:
    void readyRead();

};

#endif // THREAD_H
