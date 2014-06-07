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
//    MainWindow w;
//    w.show();

    QVector<quint8> samples;
    int step, sb, s, i, r;
    sb = 100;
    s = 97;
    r=s-1;
    step = sb / (sb-s);
    samples.resize(s);
    for(i=0;i<s;i++) samples[i] = i;
    qDebug() << samples << samples.size();
    samples.resize(sb);
    for(i=sb-1;i>=0;i--)
    {
        --r;
        if(i%step == 0) ++r ;
        if(r<0) r = 0;
        samples[i] = samples[r];
    }
    qDebug() << samples << samples.size();


    return 0;

    Connection c;
    c.setPortName("COM7");
    qDebug() << c.open();
    qDebug() << c.setChannel(1);
    qDebug() << c.setChunkSize(5000000);
    qDebug() << c.setChunkSize(200000);
    c.initReadChannel();

    //QTimer::singleShot(5000, qApp, SLOT(quit()));

    return a.exec();
}
