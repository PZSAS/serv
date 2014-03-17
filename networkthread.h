#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QSettings>
#include <QDir>
#include <QCryptographicHash>



class NetworkThread : public QThread
{
    Q_OBJECT
public:
    explicit NetworkThread(QObject *parent = 0);
    ~NetworkThread();

signals:

public slots:


public:
    void setSocket(int socketDesc);



private:

    void run();
    QByteArray generateHeaders(int code = 200, QString mime = QString());
    QByteArray fileTxtList();
    QByteArray fileXmlList();
    QByteArray infoHTML(QString title, QString info);
    QString password;
    int socketDescriptor;
    bool ready;


};

#endif // NETWORKTHREAD_H
