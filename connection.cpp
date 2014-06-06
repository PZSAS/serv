#include "connection.h"

Connection::Connection(QObject *parent) :
    QObject(parent)
{
    port = new QSerialPort;
    port->setPortName("COM7");

    newRequest = false;
    chunkSize = 1000000;
    channel = 1;

    rawSamples.reserve(8192);

    connect(port, SIGNAL(readyRead()), this, SLOT(readData()));
}

Connection::~Connection()
{
    close();
}

bool Connection::openClose()
{
    if(port->isOpen()) return close();
    return open();
}

bool Connection::open()
{
    if(port->isOpen()) return true;
    if(!port->open(QIODevice::ReadWrite))
    {
        qDebug() << "Nie można otworzyć portu";
        return false;
    }
    port->setBaudRate(115200);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    return true;
}

bool Connection::close()
{
    port->close();
    return true;
}

void Connection::setPortName(QString portName)
{
    port->setPortName(portName);
}

void Connection::initReadChannel(int channel, int probes)
{
    //'a'     [kanał]    [ilość próbek 4 bajty]    'k'
    QByteArray data;
    char ch = (char) channel;

    if(this->channel != channel) this->channel = channel;
    if(chunkSize != probes) chunkSize = probes;

    data.append('a'); // a
    data.append(ch); // kanal
    data.append(Container::int32ToByteBE(probes)); // ilosc probek
    data.append('k'); // k

    newRequest = true;
    buffer.clear();
    header.clear();
    rawSamples.clear();
    port->write(data);

}

void Connection::handleWithSamples()
{
    QVector<quint8> newSamples;
    int i, count, size;
    int freq = 100;
    qint64 sum;

    if(channel == 3) freq = 500; // mikrofon

    sum = 0;
    size = rawSamples.size();
    newSamples.reserve(4096);
    count = qRound(SAMPLES_PER_SEC/freq); // po ile uśredniać

    for(i=0;i<size;i++)
    {
        sum += rawSamples[i];
        if((i % count) == (count-1))
        {
            newSamples.append(qRound((float)sum / (float)count));
            sum = 0;
        }
    }
    if((i%count) > count/2)
    {
        newSamples.append(qRound((float)sum / (float)(i%count)));
    }

    samples << newSamples;
}

void Connection::initReadChannel()
{
    initReadChannel(channel, chunkSize);
}

bool Connection::setChannel(int channel)
{
    if(SignalAnalyzer::signalTypes.keys().indexOf((qint16) channel) < 0)
    {
        return false;
    }
    this->channel = channel;
    return true;
}

int Connection::setChunkSize(int size)
{
    if(size <= 10000) // 10 tys
        chunkSize = 10000;
    else if(size <= 100000) // 100 tys
        chunkSize = 100000;
    else if(size <= 2000000) // 200 tys
        chunkSize = 200000;
    else if(size <= 5000000) // 500 tys
        chunkSize = 500000;
    else
        chunkSize = 1000000; // milion
    return chunkSize;
}

void Connection::readData()
{
    QByteArray data;
    if(newRequest)
    {
        buffer += port->readAll();
        // czeka na wczytanie calego naglowka
        if(buffer.size() < 22) return;

        // zapisuje naglowek i przechodzi dalej
        header = buffer.mid(0, 18);
        buffer = buffer.mid(18);
        samplesRead = buffer.size();
        newRequest = false;
        return;
    }
    else
    {
        data = port->readAll();
        samplesRead += data.size();
        buffer += data;
        qDebug() << samplesRead << (chunkSize+18);
        if(samplesRead >= (chunkSize+18))
        {
            qDebug() << "koniec";
            return;
        }
        if(buffer.size() >= 4096)
        {
            rawSamples = Container::data8ToInt8Vec(buffer.mid(0, 4096));
            buffer = buffer.mid(4096);
            handleWithSamples();
        }
    }
}
