#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QTcpServer>

#include "networkthread.h"

class NetworkServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit NetworkServer(QObject *parent = 0);

signals:

protected:
    void incomingConnection(qintptr socketDescriptor);
    QString pass;

public slots:

};

#endif // NETWORKSERVER_H


