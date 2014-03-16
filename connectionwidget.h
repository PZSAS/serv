#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>

#include "container.h"

#define MAX_LOG_COUNT 200


namespace Ui {
class ConnectionWidget;
}

class ConnectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionWidget(QWidget *parent = 0);
    ~ConnectionWidget();

private slots:
    void updateStatus();
    void updatePortList();
    void openCloseConnection();
    void readData();

private:
    void log(QString message, int level=0);
    void closeConnection();
    void updateState(QString state, QString value = QString());


private:
    Ui::ConnectionWidget *ui;
    QTimer *timer;
    int availablePortCount;
    QSerialPort *port;
    QByteArray rawData;
    QByteArray startSeq;
    bool startDetected;
    QDateTime startTime;
};

#endif // CONNECTIONWIDGET_H
