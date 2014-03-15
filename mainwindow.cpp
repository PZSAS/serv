#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon icon(":/img/icon.png");
    trayIcon = new QSystemTrayIcon(icon, this);
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
