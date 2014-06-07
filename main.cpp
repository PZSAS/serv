#include "mainwindow.h"
#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("MonitorBezdechu");
    a.setOrganizationName("StudenciEAE");
    a.setOrganizationDomain("www.kmeif.pwr.wroc.pl");
    a.setWindowIcon(QIcon(":/img/icon.png"));

    MainWindow w;
    w.show();


    return a.exec();
}
