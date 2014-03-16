#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("MonitorOddechu");
    a.setOrganizationName("StudenciEAE");
    a.setOrganizationDomain("www.kmeif.pwr.wroc.pl");


    MainWindow w;
    w.show();

    return a.exec();
}
