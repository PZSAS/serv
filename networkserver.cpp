#include "networkserver.h"

NetworkServer::NetworkServer(QObject *parent) :
    QTcpServer(parent)
{
}

void NetworkServer::incomingConnection(qintptr socketDescriptor)
{
    NetworkThread *thread = new NetworkThread(this);
    thread->setSocket((int) socketDescriptor);
    thread->setPassword(pass);
    thread->start();
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}

void NetworkServer::setPass(const QString &password)
{
    pass = password;
}

