#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTime>
#include <QDateTime>

#include "container.h"
#include "plot.h"
#include "connection.h"
#include "signalanalyzer.h"

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
    void on_cancelConnectButton_clicked();
    void onOpened();
    void onClosed();
    void log(QString message, int level=0);

private:

    void closeConnection();
    void updateState(QString state, QString value = QString());



private:
    Ui::ConnectionWidget *ui;
    QTimer *timer;
    int availablePortCount;
    Connection *connection;
    QByteArray rawData;
    QByteArray startSeq;
    bool startDetected;
    QDateTime startTime;
    uint lastTransferTime;
    bool showTransferError;
};

#endif // CONNECTIONWIDGET_H
