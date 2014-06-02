#include "connectionwidget.h"
#include "ui_connectionwidget.h"

ConnectionWidget::ConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionWidget)
{
    ui->setupUi(this);
    ui->cancelConnectButton->hide();

    // serial port
    port = new QSerialPort(this);
    startSeq.fill((char) 0xFF,8);
    startDetected = false;
    updatePortList();


    // timer
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();

    // connections
    connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));
    connect(ui->updatePortListButton, SIGNAL(clicked()), this, SLOT(updatePortList()));
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(openCloseConnection()));
    connect(port, SIGNAL(readyRead()), this, SLOT(readData()));



}

ConnectionWidget::~ConnectionWidget()
{
    if(port->isOpen()) port->close();
    timer->stop();
    delete ui;
}

void ConnectionWidget::updateStatus()
{
    if(!this->isVisible()) return;
    // uaktualnienie statusu

    if(!port->isOpen())
    {
        if(!isVisible()) return;
        updateState("connection");
        updateState("dataTransfer", "--");
        updateState("deviceTime", "--");
        updateState("startTime", "--");
        updateState("elapsedTime");
        return;
    }
    updateState("elapsedTime");
    if((QDateTime::currentDateTime().toTime_t() - lastTransferTime) > 12)
    {
        QString err;
        err.append(tr("Nie otrzymano danych od "));
        err.append(QString::number(QDateTime::currentDateTime().toTime_t()-lastTransferTime));
        err.append(tr(" sekund"));
        if(showTransferError)
        {
            log(tr("Brak transmisji danych"), 3);
            showTransferError = false;
        }
        updateState("dataTransfer", err);
    }
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
    if(port->isOpen())
    {
        port->close();
        log("Rozłączono z urządzeniem", 2);
        ui->connectButton->setText(tr("Rozpocznij zapis"));
        ui->cancelConnectButton->hide();
        updateStatus();
        Container::getCurrent()->saveToFile();
        Container::getCurrent()->clear();
        return;
    }
    if(!ui->portListWidget->count())
        updatePortList();
    if(!ui->portListWidget->count())
    {
        log(tr("Brak dostępnych portów. Nie podłączono urządzenia"), 4);
        log(tr("Nie udało się połączyć"), 3);
        return;
    }

    port->setPortName(ui->portListWidget->currentText());
    if(!port->open(QIODevice::ReadWrite))
    {
        port->close();
        log(tr("Nie udało się połączyć"), 3);
        return;
    }

    //port settings
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    log(tr("Połączono z urządzeniem"), 1);
    ui->connectButton->setText(tr("Zakończ i zapisz"));
    startTime = QDateTime::currentDateTime();
    ui->cancelConnectButton->show();
    updateState("connection");
    updateState("startTime");
    Container::getCurrent()->clear();
    showTransferError = true;
    lastTransferTime = QDateTime::currentDateTime().toTime_t();
}

void ConnectionWidget::readData()
{
    static qint32 bytesToRead=0;
    int start;
    bool valid;
    rawData.append(port->readAll());
    if(!startDetected)
    {
        start = rawData.indexOf(startSeq);
        if(start<0) return;
        start += startSeq.size();
        rawData = rawData.mid(start);
        startDetected = true;
    }
    if(rawData.size() < 12) return;
    if(bytesToRead == 0 && rawData.size() >= 12)
    {
        bytesToRead = Container::byteToInt32(rawData.mid(8, 4));
    }
    if(rawData.size() >= bytesToRead)
    {
        lastTransferTime = QDateTime::currentDateTime().toTime_t();
        showTransferError = true;
        valid = Container::getCurrent()->load(rawData, true);
        if(!valid)
        {
            log(tr("Odebrano nieprawidłowe dane"), 3);
            updateState("dataTransfer", tr("Odebrano nieprawidłowe dane"));
        }
        updateState("dataTransfer", tr("OK"));
        if(qAbs(Container::byteToInt32(rawData.mid(14, 4))
                - (qint32) QDateTime::currentDateTime().toTime_t()) > 60*15)
        {
            updateState("deviceTime", tr("ponad 15 minut różnicy"));
        }
        else updateState("deviceTime", tr("prawidłowy"));
        rawData.clear();
        startDetected = false;
        bytesToRead = 0;
        start = -1;
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
    if(state == "connection")
    {
        value = tr("Połączony: ");
        if(port->isOpen())
            value.append(tr("TAK"));
        else
            value.append(tr("NIE"));
        ui->connectionStateLabel->setText(value);
    }
    else if(state == "dataTransfer")
    {
        ui->dataTransferLabel->setText(tr("Transfer danych: ").append(value));
    }
    else if(state == "deviceTime")
    {
        value = tr("Czas urządzenia: ").append(value);
        ui->deviceTimeLabel->setText(value);
    }
    else if(state == "startTime")
    {
        value = tr("Czas rozpoczęcia: ");
        if(port->isOpen()) value.append(startTime.toString("HH:mm:ss"));
        else value.append("--");
        ui->startTimeLabel->setText(value);
    }
    else if(state == "elapsedTime")
    {
        value = tr("Czas pomiaru: ");
        if(!port->isOpen())
        {
            value.append("--");
            ui->elapsedTime->setText(value);
            return;
        }
        int tmp, h, m, s;
        tmp = startTime.secsTo(QDateTime::currentDateTime());
        s = tmp%60;
        tmp = (tmp-s)/60;
        m = tmp%60;
        h = (tmp-m)/60;
        if(h) value.append(QString::number(h)).append(":");
        if(m>0 && m<10) value.append("0");
        if(m) value.append(QString::number(m)).append(":");
        if(s<10) value.append("0");
        if(s==0) value.append("0");
        if(s) value.append(QString::number(s));
        ui->elapsedTime->setText(value);
    }
}





void ConnectionWidget::on_cancelConnectButton_clicked()
{
    port->close();
    log("Rozłączono z urządzeniem bez zapisywania", 2);
    ui->connectButton->setText(tr("Rozpocznij zapis"));
    ui->cancelConnectButton->hide();
    updateStatus();
    Plot *plot = new Plot(0);
    plot->setData(Container::getCurrent(), 3);
    plot->show();
    Container::getCurrent()->clear();
    return;
}
