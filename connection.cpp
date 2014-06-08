#include "connection.h"




Connection::Connection(QObject *parent) :
    QObject(parent)
{
    int i;
    port = new QSerialPort();

    timer = new QTimer(this);
    timer->setInterval(5000);

    newRequest = false;
    started = false;
    chunkSize = 1000000;
    channel = 1;

    rawSamples.reserve(8192);

    ln = 0;
    for(i=0;i<LAST_SAMPLES_SIZE;i++) lastSamples[i] = 0;

    connect(port, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(timer, SIGNAL(timeout()), this, SLOT(checkConnection()));
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

    emit opened();
    emit log(tr("Połączono"), 1);
    return true;
}

bool Connection::close()
{
    port->close();
    started = false;
    emit closed();
    emit log(tr("Rozłączono"), 0);
    return true;
}

void Connection::setPortName(QString portName)
{
    port->setPortName(portName);
}

void Connection::initReadChannel(int channel, int probes)
{

    QByteArray data;
    char ch = (char) channel;

    if(started) return;
    if(this->channel != channel) this->channel = channel;
    if(chunkSize != probes) chunkSize = probes;

    data.append('a');
    data.append(ch);
    data.append(Container::int32ToByteBE(probes)); // ilosc probek
    data.append('k');


    newRequest = true;
    started = true;
    samplesRead = 0;
    buffer.clear();
    header.clear();
    rawSamples.clear();
    samples.clear();
    time.start();
    lastRead.start();
    timer->start();
    port->write(data);
    port->waitForBytesWritten(10);
    emit log(tr("Wysłano żądanie %1 próbek z kanału %2").arg(probes).arg(channel), 0);
}

void Connection::handleWithSamples()
{
    QVector<quint8> newSamples;
    int i, count, size;
    int freq = FREQ;
    qint64 sum;

    if(channel == MIC) freq = MIC_FREQ; // mikrofon

    sum = 0;
    size = rawSamples.size();
    newSamples.reserve(INTERVAL);
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

    size = newSamples.size();
    for(i=0;i<size;i++)
    {
        ++ln;
        ln = ln & Nmask;
        lastSamples[ln] = newSamples[i];
    }
}

void Connection::endReading(bool cont)
{
    QString str = QString::fromLocal8Bit(buffer.mid(buffer.size()-4));
    float elapsed;
    int freq, s, sb, i, r, step;
    if(str != "STOP")
    {
        emit log(tr("Brak potwierdzenia stopu"), 0);
    }
    if(buffer.size() > 4)
    {
        buffer = buffer.mid(0, buffer.size()-4);
        handleWithSamples();
    }
    str = tr("Zakończono odczyt paczki danych. Otrzymano %1 próbek w %2 s");
    str = str.arg(samplesRead).arg(QString::number(time.elapsed()/1000.0));
    emit log(str, 0);


    freq = FREQ;
    if(channel == MIC) freq = MIC_FREQ; //mikrofon
    elapsed = time.elapsed() / 1000.0;
    s = samples.size(); // ile jest
    sb = qRound(elapsed*freq); // ile powinno być
    started = false;

    // dodaj lub usun probki
    if(s < sb) // jezeli jest za malo
    {
        samples.resize(sb);
        step = sb / (sb-s);
        r=s-1;
        for(i=sb-1;i>=0;i--)
        {
            --r;
            if(i%step == 0) ++r ;
            if(r<0) r = 0;
            samples[i] = samples[r];
        }
    }
    else if(s > sb) // jezeli jest za duzo
    {
        r = 0;
        step = sb / (s-sb);
        for(i=0;i<sb;i++)
        {
            if(i%step == 0) ++r;
            if(r>=s) r = s-1;
            samples[i] = samples[r];
            ++r;
        }
        samples.resize(sb);
    }
    Container::getCurrent()->addSamples(samples);



    if(cont) initReadChannel();



}

void Connection::checkHeader()
{
    QString str = QString::fromLocal8Bit(header.mid(0, 4));
    if(str != "DATA")
    {
        emit log(tr("Odebrano nieprawidłowy nagłówek"), 3);
    }
}

void Connection::prepareContainer()
{
    SampleInfo info;
    info.sampleSize = 1;
    info.freq = FREQ;
    if(channel == MIC) info.freq = MIC_FREQ;

    Container::getCurrent()->clear();
    Container::getCurrent()->loadHeaders(channel, info);
}

void Connection::initReadChannel()
{
    initReadChannel(channel, chunkSize);
}

void Connection::startRecording()
{
    int i;
    if(!open()) return;
    // czysc bufor kolowy (dla wykresu)
    for(i=0;i<LAST_SAMPLES_SIZE;i++) lastSamples[i] = 0;
    prepareContainer();
    initReadChannel();

}

void Connection::stopAndSave()
{
    endReading(false);
    stop();


    Container::getCurrent()->saveToFile();
}

void Connection::stop()
{
    port->write("b", 1);
    port->waitForBytesWritten(15);
    close();
}

bool Connection::isOpen()
{
    return port->isOpen();
}

void Connection::sendCommand(int conf)
{
    QByteArray data;
    char ch = conf;
    if(!port->isOpen())
    {
        qDebug() << "Nie można wysłać polecenia. Port zamknięty";
        return;
    }
    data.append('w');
    data.append(ch);
    data.append('k');
    port->write(data);
    emit log(tr("Wysłano komendę ") + QString::number((int)ch, 16), 0);
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
    else if(size <= 200000) // 200 tys
        chunkSize = 200000;
    else if(size <= 500000) // 500 tys
        chunkSize = 500000;
    else
        chunkSize = 1000000; // milion
    return chunkSize;
}

QVector<qint16> Connection::getLastSamples(int size)
{
    QVector<qint16> tab;
    int i, idx;
    if(size >= LAST_SAMPLES_SIZE) size = LAST_SAMPLES_SIZE;
    tab.resize(size);

    for(i=0;i<size;i++)
    {
        idx = (ln - size + i) & Nmask;
        tab[i] = lastSamples[idx];
    }
    return tab;
}

void Connection::readData()
{
    QByteArray data;
    lastRead.start();
    if(newRequest)
    {
        buffer += port->readAll();
        // czeka na wczytanie calego naglowka
        if(buffer.size() < 18) return;

        // zapisuje naglowek i przechodzi dalej
        header = buffer.mid(0, 18);
        checkHeader();
        samplesRead = buffer.size();
        buffer = buffer.mid(18);
        newRequest = false;
    }
    else
    {
        data = port->readAll();
        samplesRead += data.size();
        buffer += data;
        if(buffer.size() >= INTERVAL && samplesRead < (chunkSize+18))
        {
            rawSamples = Container::data8ToInt8Vec(buffer.mid(0, INTERVAL));
            buffer = buffer.mid(INTERVAL);
            handleWithSamples();
        }
    }
    if(samplesRead >= (chunkSize+22))
    {
        endReading();
    }

}

void Connection::checkConnection()
{
    if(!port->isOpen())
    {
        timer->stop();
        return;
    }
    int ms = lastRead.elapsed();
    if(ms > 10000) // > 10 sekund
    {
        emit log(tr("Brak transmisji danych od %1s. Rozłączanie").arg(ms/1000), 4);
        close();
    }
}


