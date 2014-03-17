#include "networkserver.h"

NetworkServer::NetworkServer(QObject *parent) :
    QTcpServer(parent)
{
}

void NetworkServer::incomingConnection(qintptr socketDescriptor)
{
    NetworkThread *thread = new NetworkThread(this);
    thread->setSocket((int) socketDescriptor);
    thread->start();
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}

