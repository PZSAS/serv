#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTime>
#include <QHostAddress>
#include <QSettings>

#include "networkserver.h"
#include "networkthread.h"


#define MAX_LOG_COUNT 200
#define MAX_PEER_HISTORY 50

namespace Ui {
class NetworkWidget;
}

class NetworkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkWidget(QWidget *parent = 0);
    ~NetworkWidget();

private slots:
    void startStopServer();

    void on_changePassButton_clicked();

private:
    void log(QString message, int level=0);


private:
    Ui::NetworkWidget *ui;
    NetworkServer server;
    QStringList peerList;
};

#endif // NETWORKWIDGET_H
