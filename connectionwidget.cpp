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
    ui->plot->setData(QVector<qint16>(20), 10);


    // timer
    timer = new QTimer(this);
    timer->setInterval(500);

    // connections
    connect(ui->updatePortListButton, SIGNAL(clicked()), this, SLOT(updatePortList()));
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(openCloseConnection()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePlot()));

    connect(connection, SIGNAL(closed()), this, SLOT(onClosed()));
    connect(connection, SIGNAL(opened()), this, SLOT(onOpened()));
    connect(connection, SIGNAL(log(QString,int)), this, SLOT(log(QString,int)));



}

ConnectionWidget::~ConnectionWidget()
{
    connection->close();
    delete ui;
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

void ConnectionWidget::updatePlot()
{
    ui->plot->setData(connection->getLastSamples(5000), 50);
    ui->plot->repaint();
}



void ConnectionWidget::onOpened()
{
    ui->connectButton->setText(tr("Zakończ i zapisz"));
    ui->cancelConnectButton->show();
    ui->portListWidget->setEnabled(false);
    ui->channelWidget->setEnabled(false);
    timer->start();
}

void ConnectionWidget::onClosed()
{
    timer->stop();
    ui->connectButton->setText(tr("Rozpocznij zapis"));
    ui->cancelConnectButton->hide();
    ui->portListWidget->setEnabled(true);
    ui->channelWidget->setEnabled(true);
}





void ConnectionWidget::on_cancelConnectButton_clicked()
{
    connection->close();
    Container::getCurrent()->clear();
}
