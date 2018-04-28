#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>

class Communicator : public QObject
{
    Q_OBJECT

public:
    Communicator(QObject *parent = 0);
    ~Communicator();
    void sendMessage(const char *message);

private:
    QTcpServer *server;
    int listenPort = 9999;

private slots:
    void newConnection();

signals:
    void connected();
};

#endif // COMMUNICATOR_H
