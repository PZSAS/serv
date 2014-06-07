#include "connectionwidget.h"
#include "ui_connectionwidget.h"

ConnectionWidget::ConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionWidget)
{
    ui->setupUi(this);
    ui->cancelConnectButton->hide();


    connection = new Connection();
    updatePortList();
    ui->channelWidget->addItems(SignalAnalyzer::listOfSignals());
    if(ui->channelWidget->count() > 1)ui->channelWidget->setCurrentIndex(1);


    // timer
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();

    // connections
    connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
    connect(ui->updatePortListButton, SIGNAL(clicked()), this, SLOT(updatePortList()));
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(openCloseConnection()));

    connect(connection, SIGNAL(closed()), this, SLOT(onClosed()));
    connect(connection, SIGNAL(opened()), this, SLOT(onOpened()));
    connect(connection, SIGNAL(log(QString,int)), this, SLOT(log(QString,int)));



}

ConnectionWidget::~ConnectionWidget()
{
    connection->close();
    delete ui;
}

void ConnectionWidget::updateStatus()
{
//    if(!this->isVisible()) return;
//    // uaktualnienie statusu

//    if(!port->isOpen())
//    {
//        if(!isVisible()) return;
//        updateState("connection");
//        updateState("dataTransfer", "--");
//        updateState("deviceTime", "--");
//        updateState("startTime", "--");
//        updateState("elapsedTime");
//        return;
//    }
//    updateState("elapsedTime");
//    if((QDateTime::currentDateTime().toTime_t() - lastTransferTime) > 12)
//    {
//        QString err;
//        err.append(tr("Nie otrzymano danych od "));
//        err.append(QString::number(QDateTime::currentDateTime().toTime_t()-lastTransferTime));
//        err.append(tr(" sekund"));
//        if(showTransferError)
//        {
//            log(tr("Brak transmisji danych"), 3);
//            showTransferError = false;
//        }
//        updateState("dataTransfer", err);
//    }
}

void ConnectionWidget::updatePortList()
{
    QString portName;
    int portCount;
    int logLevel;
    ui->portListWidget->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        portName = info.portName();
#ifndef Q_OS_WIN
        portName = QString("/dev/").append(portName);
#endif
        ui->portListWidget->addItem(portName);
    }
    portCount = ui->portListWidget->count();
    logLevel = 0;
    if(portCount < 1) logLevel = 2;
    log(tr("Lista portów zakutalizowana. Dostępnych portów: %1").arg(portCount),logLevel);
}

void ConnectionWidget::openCloseConnection()
{
    if(connection->isOpen())
    {
        connection->stopAndSave();
    }
    else
    {
        connection->setPortName(ui->portListWidget->currentText());
        connection->setChannel(ui->channelWidget->currentIndex());
        connection->setChunkSize(200000);
        connection->startRecording();
    }
}



void ConnectionWidget::log(QString message, int level)
{
    int row;

    if(ui->logList->count() >= MAX_LOG_COUNT)
        ui->logList->takeItem(0);

    message = QTime::currentTime().toString("[HH:mm:ss] ") + message;
    ui->logList->addItem(message);
    row = ui->logList->count()-1;
    switch(level)
    {
        case 1:
            ui->logList->item(row)->setForeground(Qt::green);
        break;
        case 2:
            ui->logList->item(row)->setForeground(Qt::blue);
        break;
        case 3:
            ui->logList->item(row)->setForeground(Qt::red);
        break;
        case 4:
            ui->logList->item(row)->setBackground(Qt::red);
        break;
        default:
        break;

    }
    ui->logList->scrollToBottom();
}

void ConnectionWidget::updateState(QString state, QString value)
{
//    if(state == "connection")
//    {
//        value = tr("Połączony: ");
//        if(port->isOpen())
//            value.append(tr("TAK"));
//        else
//            value.append(tr("NIE"));
//        ui->connectionStateLabel->setText(value);
//    }
//    else if(state == "dataTransfer")
//    {
//        ui->dataTransferLabel->setText(tr("Transfer danych: ").append(value));
//    }
//    else if(state == "deviceTime")
//    {
//        value = tr("Czas urządzenia: ").append(value);
//        ui->deviceTimeLabel->setText(value);
//    }
//    else if(state == "startTime")
//    {
//        value = tr("Czas rozpoczęcia: ");
//        if(port->isOpen()) value.append(startTime.toString("HH:mm:ss"));
//        else value.append("--");
//        ui->startTimeLabel->setText(value);
//    }
//    else if(state == "elapsedTime")
//    {
//        value = tr("Czas pomiaru: ");
//        if(!port->isOpen())
//        {
//            value.append("--");
//            ui->elapsedTime->setText(value);
//            return;
//        }
//        int tmp, h, m, s;
//        tmp = startTime.secsTo(QDateTime::currentDateTime());
//        s = tmp%60;
//        tmp = (tmp-s)/60;
//        m = tmp%60;
//        h = (tmp-m)/60;
//        if(h) value.append(QString::number(h)).append(":");
//        if(m>0 && m<10) value.append("0");
//        if(m) value.append(QString::number(m)).append(":");
//        if(s<10) value.append("0");
//        if(s==0) value.append("0");
//        if(s) value.append(QString::number(s));
//        ui->elapsedTime->setText(value);
//    }
}

void ConnectionWidget::onOpened()
{
    ui->connectButton->setText(tr("Zakończ i zapisz"));
    ui->cancelConnectButton->show();
}

void ConnectionWidget::onClosed()
{
    ui->connectButton->setText(tr("Rozpocznij zapis"));
    ui->cancelConnectButton->hide();
}





void ConnectionWidget::on_cancelConnectButton_clicked()
{
    connection->close();
    Container::getCurrent()->clear();
}
