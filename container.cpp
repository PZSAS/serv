#include "container.h"

Container::Container(QObject *parent) :
    QObject(parent)
{
    loaded = false;
}

Container *Container::current=0;

qint32 Container::byteToInt32(QByteArray &data)
{
    if(data.size() != 4) return 0;
    char *tab = data.data();
    void *ind = tab;
    qint32 *number;
    number = static_cast <qint32 *> (ind);
    return *number;
}

qint16 Container::byteToInt16(QByteArray &data)
{
    if(data.size() != 2) return 0;
    char *tab = data.data();
    void *ind = tab;
    qint16 *number;
    number = static_cast <qint16 *> (ind);
    return *number;
}

quint8 Container::byteToUInt8(QByteArray &data)
{
    if(data.size() != 1) return 0;
    char *tab = data.data();
    void *ind = tab;
    quint8 *number;
    number = static_cast <quint8 *> (ind);
    return *number;
}

QByteArray Container::int32ToByteBE(qint32 number)
{

    QByteArray data;
    void *p;
    char *c;
    p = &number;
    c = static_cast <char *> (p);
    data.append(c[3]);
    data.append(c[2]);
    data.append(c[1]);
    data.append(c[0]);
    return data;


}

QByteArray Container::int32ToByte(qint32 number)
{
    QByteArray data;
    void *p;
    char *c;
    p = &number;
    c = static_cast <char *> (p);
    data.setRawData(c, 4);
    return data;

}

QByteArray Container::int16ToByte(qint16 number)
{
    QByteArray data;
    void *p;
    char *c;
    p = &number;
    c = static_cast <char *> (p);
    data.setRawData(c, 2);
    return data;
}

QByteArray Container::uint8ToByte(quint8 number)
{
    QByteArray data;
    void *p;
    char *c;
    p = &number;
    c = static_cast <char *> (p);
    data.setRawData(c, 1);
    return data;
}

QByteArray Container::int16VecToByte16(QVector<qint16> tab)
{
    QByteArray date;
    int i, max = tab.size();
    for(i=0;i<max;i++)
    {
        date.append(int16ToByte(tab[i]));
    }
    return date;
}

QByteArray Container::int16VecToByte8(QVector<qint16> tab)
{
    QByteArray date;
    int i, max = tab.size();
    for(i=0;i<max;i++)
    {
        date.append(uint8ToByte((quint8)tab[i]));
    }
    return date;
}

QVector<qint16> Container::dataToInt16Vec(QByteArray &data)
{
    QVector<qint16> tab;
    int max, i;
    qint16 *t;
    qint16 number;
    char *d;
    void *v;
    if(data.size() % 2 == 1)
    {
        qDebug() << "zle dane";
        return tab;
    }
    max = data.size()/2;
    tab.resize(max);
    d = data.data();
    v = d;
    t = static_cast <qint16 *> (v);
    for(i=0;i<max;i++)
    {
        number = *t;
        tab[i] = number;
        t++;
    }
    return tab;

}

QVector<qint16> Container::data8ToInt16Vec(QByteArray &data)
{
    QVector<qint16> tab;
    int max, i;
    quint8 *t;
    quint8 number;
    char *d;
    void *v;
    if(data.size() % 2 == 1)
    {
        qDebug() << "zle dane" << data.size();
        return tab;
    }
    max = data.size();
    tab.resize(max);
    d = data.data();
    v = d;
    t = static_cast <quint8 *> (v);
    for(i=0;i<max;i++)
    {
        number = *t;
        tab[i] = (qint16) number;
        t++;
    }
    return tab;
}

QVector<quint8> Container::data8ToInt8Vec(QByteArray &data)
{
    QVector<quint8> tab;
    int max, i;
    quint8 *t;
    quint8 number;
    char *d;
    void *v;
    if(data.size() % 2 == 1)
    {
        qDebug() << "zle dane";
        return tab;
    }
    max = data.size();
    tab.resize(max);
    d = data.data();
    v = d;
    t = static_cast <quint8 *> (v);
    for(i=0;i<max;i++)
    {
        number = *t;
        tab[i] = (qint8) number;
        t++;
    }
    return tab;
}

bool Container::validateData(QByteArray data)
{
    QByteArray pattern;
    int i, offset;
    qint32 rozmiarPliku, liczbaSekund, liczbaProbek;
    qint16 liczbaSygnalow, id, czestotliwosc, rozmiarProbki;
    pattern.setRawData("FPDM", 4);
    if(data.mid(0,4) != pattern)
    {
        qDebug() << "FPDM";
        return false;
    }
    rozmiarPliku = byteToInt32(data.mid(8,4));
    if(rozmiarPliku != data.size())
    {
        qDebug() << "rozmiarPliku";
        return false;
    }
    liczbaSygnalow = byteToInt16(data.mid(12,2));
    if(liczbaSygnalow < 1 || liczbaSygnalow > 50)
    {
        qDebug() << "liczbaSygnalow";
        return false;
    }
    liczbaSekund = byteToInt32(data.mid(14,4));

    offset = 26;

    for(i=0;i<liczbaSygnalow;i++)
    {
        pattern.setRawData("DATA",4);
        if(data.mid(offset,4) != pattern)
        {
            qDebug() << "DATA" << i;
            return false;
        }
        id = byteToInt16(data.mid(offset+4,2));
        czestotliwosc = byteToInt16(data.mid(offset+6,2));
        rozmiarProbki = byteToInt16(data.mid(offset+8,2));
        if(rozmiarProbki != 1 && rozmiarProbki != 2 && rozmiarProbki != 4)
        {
            qDebug() << "rozmiarProbki" << i;
            return false;
        }
        liczbaProbek = byteToInt32(data.mid(offset+10,4));
        if(liczbaSekund != (liczbaProbek/czestotliwosc))
        {
            qDebug() << "liczbaSekund" << i;
            qDebug() << "sekundy" << liczbaSekund << "r probki" << rozmiarProbki
                         << "l probek" << liczbaProbek  << "cz" << czestotliwosc;
            return false;
        }
        pattern.setRawData("STOP",4);
        offset = offset + 18 + liczbaProbek*rozmiarProbki;
        if(data.mid(offset,4) != pattern)
        {
            qDebug() << "STOP" << i;
            return false;
        }
        offset += 4;

    }

    return true;
}

FileInfo Container::fileInfo(QString fileName, bool absolutePath)
{
    QFile file;
    QDir dir;
    FileInfo fileInfo;
    QByteArray data;

    fileInfo.duration = 0;
    fileInfo.signalsCount = 0;
    fileInfo.startDate = QDateTime();

    if(!absolutePath)
    {
        QSettings settings;
        dir.setPath(settings.value("dataDir").toString());
        fileName = dir.absoluteFilePath(fileName);
    }
    file.setFileName(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "nie ma takiego pliku";
        return fileInfo;
    }
    data = file.read(18);
    file.close();
    if(data.length() != 18)
    {
        qDebug() << "nieprawidlowy plik";
        return fileInfo;
    }

    // timestamp
    fileInfo.startDate = QDateTime::fromTime_t(byteToInt32(data.mid(4,4)));

    // liczbaSekund
    fileInfo.duration = byteToInt32(data.mid(14,4));

    // liczbaSygnalow
    fileInfo.signalsCount = byteToInt32(data.mid(12,2));

    return fileInfo;


}

Container *Container::getCurrent()
{
    if(current == NULL)
    {
        current = new Container;
    }
    return current;
}

QMap<qint16, QVector<qint16> > Container::getSamples()
{
    if(!loaded)
    {
        QMap<qint16, QVector<qint16> > empty;
        return empty;
    }
    return samples;
}

QMap<qint16, SampleInfo> Container::getSamplesInfo()
{
    if(!loaded)
    {
        QMap<qint16, SampleInfo> empty;
        return empty;
    }
    return samplesInfo;
}

QVector<qint16> Container::getSamplesFromIndex(qint16 idx)
{
    return samples.value(idx);
}

SampleInfo Container::getSamplesInfoFromIndex(qint16 idx)
{
    return samplesInfo.value(idx);
}



bool Container::contains(qint16 idx)
{
    return (samplesInfo.contains(idx) && samples.contains(idx));
}



bool Container::load(QByteArray &data, bool append)
{
    QDateTime startTime;
    QMap<qint16, SampleInfo> lSamplesInfo;
    QMap<qint16, QVector<qint16> > lSamples;
    QVector<qint16> tab;
    QVector<qint16> tmpTab;
    QMap<qint16, SampleInfo>::iterator irf;
    QMap<qint16, QVector<qint16> >::iterator ir;


    QByteArray pattern;
    int i, offset;
    qint32 rozmiarPliku, liczbaSekund, liczbaProbek, timestamp;
    qint16 liczbaSygnalow, id, czestotliwosc, rozmiarProbki;
    quint16 iterator;
    quint16 brakujacychRamek;
    pattern.setRawData("FPDM", 4);
    if(data.mid(0,4) != pattern)
    {
        qDebug() << "FPDM";
        return false;
    }
    timestamp = byteToInt32(data.mid(4,4));
    startTime = QDateTime::fromTime_t((uint) timestamp);
    rozmiarPliku = byteToInt32(data.mid(8,4));
    if(rozmiarPliku != data.size())
    {
        qDebug() << "rozmiarPliku" << rozmiarPliku << data.size();
        return false;
    }
    liczbaSygnalow = byteToInt16(data.mid(12,2));
    if(liczbaSygnalow < 1 || liczbaSygnalow > 50)
    {
        qDebug() << "liczbaSygnalow";
        return false;
    }
    liczbaSekund = byteToInt32(data.mid(14,4));
    iterator = (quint16) byteToInt16(data.mid(18,2));

    offset = 26;

    for(i=0;i<liczbaSygnalow;i++)
    {
        pattern.setRawData("DATA",4);
        if(data.mid(offset,4) != pattern)
        {
            qDebug() << "DATA" << i;
            return false;
        }
        id = byteToInt16(data.mid(offset+4,2));
        czestotliwosc = byteToInt16(data.mid(offset+6,2));
        rozmiarProbki = byteToInt16(data.mid(offset+8,2));
        lSamplesInfo[id].freq = czestotliwosc;
        lSamplesInfo[id].sampleSize = rozmiarProbki;
        if(rozmiarProbki != 1 && rozmiarProbki != 2 && rozmiarProbki != 4)
        {
            qDebug() << "rozmiarProbki" << i;
            return false;
        }
        liczbaProbek = byteToInt32(data.mid(offset+10,4));
        if(liczbaSekund != (liczbaProbek/czestotliwosc))
        {
            qDebug() << "liczbaSekund" << i;
            qDebug() << "sekundy" << liczbaSekund << "r probki" << rozmiarProbki
                         << "l probek" << liczbaProbek  << "cz" << czestotliwosc;
            return false;
        }
        if(rozmiarProbki == 1)
            tab = data8ToInt16Vec(data.mid(offset+18, rozmiarProbki*liczbaProbek));
        else
            tab = dataToInt16Vec(data.mid(offset+18, rozmiarProbki*liczbaProbek));
        if(tab.size() < 1)
        {
            qDebug() << "brak danych";
            return false;
        }
        lSamples[id] = tab;
        pattern.setRawData("STOP",4);
        offset = offset + 18 + liczbaProbek*rozmiarProbki;
        if(data.mid(offset,4) != pattern)
        {
            qDebug() << "STOP" << i;
            return false;
        }
        offset += 4;

    }



    if(append && loaded)
    {
        if(samples.size() != lSamples.size() || samplesInfo.size() != lSamplesInfo.size())
        {
            qDebug() << "zle rozmiary";
            return false;
        }

        if(lastIterator >= iterator)
        {
            qDebug() << "zly identyfikator paczki";
            return false;
        }

        // sprawdza czy otrzymane dane sa tego typu co dotychczasowe
        for (irf = samplesInfo.begin(); irf != samplesInfo.end(); ++irf)
        {
            if(irf.value().freq != lSamplesInfo[irf.key()].freq)
            {
                qDebug() << "rozne czestotliwosci";
                return false;
            }
            else if(irf.value().sampleSize != lSamplesInfo[irf.key()].sampleSize)
            {
                qDebug() << "rozne rozmiary probek";
                return false;
            }
        }
        // dodaje nowe dane na koncu aktualnych danych
        brakujacychRamek = iterator - lastIterator - 1;
        if(brakujacychRamek > 0)
        {
            if(brakujacychRamek > 1440) // 2 godziny
            {
                qDebug() << "Utracono " << brakujacychRamek
                         << " ramek. Transmisja zakończona";
                return false;
            }
            qDebug() << QDateTime::currentDateTime().toString("HH:mm:ss")
                     << "Utracono " << brakujacychRamek << " ramek";
        }
        for (ir = samples.begin(); ir != samples.end(); ++ir)
        {
            tab = ir.value();
            if(brakujacychRamek > 0)
            {
                // uzupelnia zerami sygnal w przypadku straty ramki
                // liczba elementow = freq * 5 * liczbaBrakujacychRamek
                tmpTab.fill(0,samplesInfo[ir.key()].freq * 5 * brakujacychRamek);
                tab << tmpTab;
            }
            tab << lSamples[ir.key()];
            samples[ir.key()] = tab;
        }
        durationTime += liczbaSekund;
        lastIterator = iterator;
        if(this == current) emit currentUpdated();

    }
    else
    {
        samples.clear();
        samplesInfo.clear();
        this->startTime = startTime;
        samples = lSamples;
        samplesInfo = lSamplesInfo;
        durationTime = liczbaSekund;
        loaded = true;
        lastIterator = iterator;
    }

    return true;
}

void Container::loadHeaders(qint16 channel, SampleInfo info)
{
    clear();
    this->startTime = QDateTime::currentDateTime();
    samples[channel] = QVector<qint16>();
    samplesInfo[channel] = info;
    durationTime = -1;
    loaded = true;
}

bool Container::loadFromFile(QString fileName, bool absolutePath)
{
    QFile file;
    QByteArray buffer;

    clear(); // remove all data from container

    if(!absolutePath)
    {
        QSettings settings;
        QDir dir;
        dir.setPath(settings.value("dataDir").toString());
        fileName = dir.absoluteFilePath(fileName);
    }
    file.setFileName(fileName);
    if(file.size()> 128*1024*1024) // 128MB
    {
        qDebug() << "Plik jest za duzy";
        return false;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Nie mozna otworzyc pliku";
        return false;
    }
    buffer = file.read(128*1024*1024); // 128MB
    file.close();
    return load(buffer);
}

void Container::addSamples(QVector<quint8> sig)
{
    if(!loaded) return;
    if(samples.keys().size() != 1) return;
    QVector<qint16> tmp;
    int i, size, key;
    tmp.resize(sig.size());
    size = sig.size();
    for(i=0;i<size;i++) tmp[i] = (qint16) sig[i];
    key = samples.keys().value(0);
    samples[key] << tmp;

    float freq = 100;
    if(key == 2) freq = 500;
    durationTime = samples[key].size() / freq;
}

void Container::clear()
{
    loaded = false;
    durationTime = 0;
    startTime = QDateTime();
    samples.clear();
    samplesInfo.clear();
}



bool Container::saveToFile(QString fileName)
{
    if(!loaded) return false;

    qint32 rozmiarPliku;
    SampleInfo sampleInfo;
    QMap<qint16, QVector<qint16> >::iterator ir;

    if(fileName.length() < 1)
    {
        QSettings settings;
        QDir dir;
        fileName = startTime.toString("ddMMMMhh-mm").append(".fmd");
        dir.setPath(settings.value("dataDir").toString());
        if(!dir.exists())
        {
            qDebug() << "katalog docelowy nie istnieje";
            return false;
        }
        fileName = dir.absoluteFilePath(fileName);
    }


    QFile file(fileName);
    QByteArray buffer;


    // usuniecie nadmiarowych probek
    for(ir = samples.begin(); ir != samples.end() ; ++ir)
    {
        int n;
        int f = samplesInfo[ir.key()].freq;
        if(ir.value().size() % f != 0)
        {
            n = ir.value().size() - (ir.value().size() % f);
            samples[ir.key()].resize(n);
        }
    }


    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        file.close();
        qDebug() << "nie mozna otworzyc pliku";
        return false;
    }
    // FPDM
    buffer.setRawData("FPDM", 4);
    file.write(buffer);

    // timestamp
    file.write(int32ToByte((qint32) startTime.toTime_t()));

    // rozmiarPliku
    rozmiarPliku = 26 + (samples.size() *  22);
    for(ir = samples.begin(); ir != samples.end() ; ++ir)
    {
        rozmiarPliku += ir.value().size() * samplesInfo[ir.key()].sampleSize;
    }
    file.write(int32ToByte(rozmiarPliku));

    // liczbaSygnalow
    file.write(int16ToByte((qint16) samples.size()));

    // liczbaSekund
    file.write(int32ToByte(durationTime));

    // zarezerwowane
    buffer.fill((char) 0, 8);
    file.write(buffer);

    // zapis sygnalow
    for(ir = samples.begin(); ir != samples.end() ; ++ir)
    {
        sampleInfo = samplesInfo[ir.key()];
        // DATA
        buffer.setRawData("DATA", 4);
        file.write(buffer);

        // id
        file.write(int16ToByte(ir.key()));

        // czestotliwosc
        file.write(int16ToByte(sampleInfo.freq));

        // rozmiarProbki
        file.write(int16ToByte(sampleInfo.sampleSize));

        // liczbaProbek
        file.write(int32ToByte((qint32) ir.value().size()));

        // zarezerwowane
        buffer.fill((char) 0, 4);
        file.write(buffer);

        // sygnal
        if(sampleInfo.sampleSize == 1)
        {
            file.write(int16VecToByte8(ir.value()));
        }
        else
        {
            file.write(int16VecToByte16(ir.value()));
        }

        // STOP
        buffer.setRawData("STOP", 4);
        file.write(buffer);

    }
    file.close();
    clear();

    return true;
}

int Container::getDurationTime()
{
    if(!loaded) return 0;
    return durationTime;
}

QDateTime Container::getStartTime()
{
    if(!loaded) return QDateTime();
    return startTime;
}

bool Container::isLoaded()
{
    return loaded;
}




