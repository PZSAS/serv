#include "networkwidget.h"
#include "ui_networkwidget.h"

NetworkWidget::NetworkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkWidget)
{
    ui->setupUi(this);
}

NetworkWidget::~NetworkWidget()
{
    delete ui;
}
