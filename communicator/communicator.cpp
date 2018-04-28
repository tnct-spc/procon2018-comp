#include "communicator.h"

Communicator::Communicator(QObject *parent)
    : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, &Communicator::newConnection);

    if(!server->listen(QHostAddress::Any, listenPort)) std::cout << "Server could not start";
    else std::cout << "Server started at " << std::to_string(listenPort);
}

Communicator::~Communicator()
{
    delete server;
}

void Communicator::newConnection()
{
    emit connected();
}

void Communicator::sendMessage(const char *message)
{
    QTcpSocket *socket = server->nextPendingConnection();

    socket->write(message);
    socket->flush();
    socket->waitForBytesWritten();
    socket->close() ;
}

//void Communicator::newConnection()
//{
//    QTcpSocket *socket = server->nextPendingConnection();

//    socket->write("Hello World\n");
//    socket->flush();
//    socket->waitForBytesWritten();
//    socket->close();
//}
