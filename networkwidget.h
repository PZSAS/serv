#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTime>


#define MAX_LOG_COUNT 200

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

private:
    void log(QString message, int level=0);

private:
    Ui::NetworkWidget *ui;
    QTcpServer *tcpServer;
};

#endif // NETWORKWIDGET_H
