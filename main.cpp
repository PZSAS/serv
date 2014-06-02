#include "mainwindow.h"
#include <QApplication>
#include "plot.h"
#include "signalanalyzer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("MonitorBezdechu");
    a.setOrganizationName("StudenciEAE");
    a.setOrganizationDomain("www.kmeif.pwr.wroc.pl");
    a.setWindowIcon(QIcon(":/img/icon.png"));
    //a.setStyle(QStyleFactory::create("Fusion"));


    //SignalAnalyzer q;
    //qDebug() << q.loadFromFile("04luty.fad");
    //return 0 ;
    MainWindow w;
    w.show();

    return a.exec();
}
