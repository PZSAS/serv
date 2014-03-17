#include "networkwidget.h"
#include "ui_networkwidget.h"

NetworkWidget::NetworkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkWidget)
{
    ui->setupUi(this);


    connect(ui->serverButton, SIGNAL(clicked()), this, SLOT(startStopServer()));
}

NetworkWidget::~NetworkWidget()
{

    if(server.isListening())
    {
        server.close();
    }
    delete ui;
}

void NetworkWidget::log(QString message, int level)
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
}

void NetworkWidget::startStopServer()
{

    if(server.isListening())
    {
        server.close();
        ui->serverButton->setText(tr("Włącz udostępnianie"));
        log(tr("Wyłączono udostępnianie"), 2);
        return;
    }
    if(!server.listen(QHostAddress::Any, ui->portWidget->value()))
    {
        log(tr("Nie można uruchomić serwera"), 4);
    }
    server.setPass(ui->serverPassLabel->text());
    log(tr("Uruchomiono "), 1);
    ui->serverButton->setText(tr("Wyłącz udostępnianie"));
}


