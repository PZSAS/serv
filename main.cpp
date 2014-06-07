#include "mainwindow.h"
#include <QApplication>
#include "plot.h"
#include "signalanalyzer.h"
#include "connection.h"

#include <QTimer>

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



//    Connection *c = new Connection();
//    c->setPortName("COM7");
//    qDebug() << c->open();
//    qDebug() << c->setChannel(1);
//    qDebug() << c->setChunkSize(5000000);
//    qDebug() << c->setChunkSize(200000);
//    c->startRecording();

//    QTimer::singleShot(20000, c, SLOT(stopAndSave()));
//    QTimer::singleShot(20500, qApp, SLOT(quit()));

    return a.exec();
}
