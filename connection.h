#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QObject>
#include <QSerialPort>

#include "container.h"
#include "signalanalyzer.h"

#define SAMPLES_PER_SEC 11555.35f

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);
    ~Connection();

    bool openClose();
    bool open();
    bool close();
    void setPortName(QString portName);
    void initReadChannel();
    bool setChannel(int channel);
    int setChunkSize(int size);

private:
    void initReadChannel(int channel, int probes);
    void handleWithSamples();

signals:

public slots:

private slots:
    void readData();

private:
    QSerialPort *port;
    QString portName;
    QByteArray buffer;
    QByteArray header;
    QVector<quint8> rawSamples;
    QVector<quint8> samples;
    bool newRequest;
    int samplesRead;
    int chunkSize;
    int channel;
};

#endif // CONNECTION_H
