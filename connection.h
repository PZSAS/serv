#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QObject>
#include <QTimer>
#include <QByteArray>
#include <QSerialPort>

#include "container.h"
#include "signalanalyzer.h"

#define SAMPLES_PER_SEC 11555.35f
#define INTERVAL 4096
#define FREQ 100
#define MIC_FREQ 500

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
    void startRecording();
    void stopAndSave();
    bool isOpen();
    bool setChannel(int channel);
    int  setChunkSize(int size);

private:
    void initReadChannel(int channel, int probes);
    void handleWithSamples();
    void endReading(bool cont = true);
    void checkHeader();
    void prepareContainer();

signals:
    void log(QString message, int level);
    void closed();
    void opened();

public slots:


private slots:
    void readData();
    void checkConnection();

private:
    QSerialPort *port;
    QString portName;
    QByteArray buffer;
    QByteArray header;
    QVector<quint8> rawSamples;
    QVector<quint8> samples;
    QTimer *timer;
    QTime time;
    QTime lastRead;
    bool newRequest;
    bool started;
    int samplesRead;
    int chunkSize;
    int channel;
};

#endif // CONNECTION_H
