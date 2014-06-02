#include "signalanalyzer.h"

SignalAnalyzer::SignalAnalyzer(QObject *parent) :
    QObject(parent)
{
    loaded = false;
}


bool SignalAnalyzer::test()
{
    QSettings settings;
    QFile file(settings.value("dataDir").toString().append("/04luty.fad"));
    int fileSize;
    int i;
    int typesCount;

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "nie udało się otworzyć";
        return false;
    }
    qDebug() << file.read(4);
    qDebug() << "timestamp: " << QDateTime::fromTime_t(Container::byteToInt32(file.read(4)));
    fileSize = Container::byteToInt32(file.read(4));
    qDebug() << "filesize: " << fileSize;
    typesCount = Container::byteToInt16(file.read(2));
    qDebug() << "typesCount: " << typesCount;
    if(typesCount > 10)
    {
        qDebug() << "za duzo";
        return false;
    }

    for(i=0;i<typesCount;i++)
    {
        qDebug() << "id: " << Container::byteToInt16(file.read(2));
        qDebug() << "liczba: " << Container::byteToInt32(file.read(4));
        qDebug() << "Sredni czas: " << Container::byteToInt32(file.read(4))/1000.0;
    }
    while(!file.atEnd())
    {
        qDebug() << "typ: " << Container::byteToInt16(file.read(2));
        qDebug() << "czas startu: " << Container::byteToInt32(file.read(4))/1000.0;
        qDebug() << "czas trwania: " << Container::byteToInt32(file.read(4))/1000.0;
    }
    return true;





}

bool SignalAnalyzer::loadFromFile(QString fileName, bool absolutePath)
{
    QSettings settings;
    QFileInfo fileInfo;
    QFile file;
    qint64 fileSize;
    int tmp, minSize, i;
    int eventTypeCount;
    StatInfo statInfo;
    EventInfo eventInfo;
    qint16 idx;

    loaded = false;

    if(!absolutePath)
    {
        fileName = settings.value("dataDir").toString() + "/" + fileName;
    }
    fileInfo.setFile(fileName);
    fileSize = fileInfo.size();
    if(fileSize < 14)
    {
        qDebug() << "plik nie istnieje";
        return false;
    }
    if(((fileSize-14)%10 != 0))
    {
        qDebug() << "nieprawidlowy rozmiar";
        return false;
    }

    file.setFileName(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Nie można otworzyć pliku";
        return false;
    }
    if(file.read(4) != "FPDA")
    {
        qDebug() << "Nieprawidlowy plik - brak FPDA";
        file.close();
        return false;
    }
    startTime = QDateTime::fromTime_t(Container::byteToInt32(file.read(4)));

    tmp = Container::byteToInt32(file.read(4));
    if(tmp != fileSize)
    {
        file.close();
        qDebug() << "Nie zgadza sie rozmiar pliku";
        return false;
    }
    eventTypeCount = Container::byteToInt16(file.read(2));
    if( eventTypeCount<0 || eventTypeCount > 1000)
    {
        file.close();
        qDebug() << "Zla liczba zdarzen";
        return false;
    }
    minSize = 14 + eventTypeCount*10;

    if(fileSize < minSize)
    {
        qDebug() << "Plik uszkodzony, za malo danych";
        file.close();
        return false;
    }
    for(i=0;i<eventTypeCount;i++)
    {
        idx = Container::byteToInt16(file.read(2));
        statInfo.count = Container::byteToInt32(file.read(4));
        statInfo.meanTime = Container::byteToInt32(file.read(4));
        stats[idx] = statInfo;
    }
    while(!file.atEnd())
    {
        eventInfo.type = Container::byteToInt16(file.read(2));
        eventInfo.startTime = Container::byteToInt32(file.read(4));
        eventInfo.durationTime = Container::byteToInt32(file.read(4));
        events.append(eventInfo);
    }
    file.close();
    loaded = true;

    return true;
}

bool SignalAnalyzer::isSignalKnown(qint16 idx)
{
    return signalTypes.contains(idx);
}

bool SignalAnalyzer::isEventKnown(qint16 idx)
{
    return eventTypes.contains(idx);
}

QStringList SignalAnalyzer::listOfTypes()
{
    QStringList list;
    QList<qint16> types;
    QList<qint16> keys;

    if(!loaded) return list;

    int size = events.size();
    int i;
    for(i=0;i<size;++i)
    {
        // jezeli typu jeszcze nie bylo to dodaj na liste
        if(types.indexOf(events.at(i).type) < 0)
        {
            types.append(events.at(i).type);
        }
    }
    keys = signalTypes.keys();
    size = keys.size();
    for(i=0;i<size;i++)
    {
        // jezeli typ jest nie znany to pomin
        if(types.indexOf(keys.at(i)) < 0) continue;
        list.append(signalTypes.value(keys.at(i)));
    }
    return list;
}

QList<qint16> SignalAnalyzer::indexesOfTypes()
{
    QList<qint16> types;

    if(!loaded) return types;

    int size = events.size();
    int i;
    for(i=0;i<size;++i)
    {
        // jezeli typu jeszcze nie bylo to dodaj na liste
        if(types.indexOf(events.at(i).type) < 0)
        {
            types.append(events.at(i).type);
        }
    }
    return types;
}

QMap<qint16, QString> SignalAnalyzer::initSignalType()
{
    QMap<qint16, QString> map;
    map.insert(0, tr("Pasek 1"));
    map.insert(1, tr("Pasek 2"));
    map.insert(2, tr("Mikrofon"));
    map.insert(3, tr("Termopara"));
    map.insert(4, tr("Napięcie baterii"));
    return map;
}

const QMap<qint16, QString> SignalAnalyzer::signalTypes = initSignalType();

QMap<qint16, QString> SignalAnalyzer::initEventType()
{
    QMap<qint16, QString> map;
    map.insert(1, tr("Wdech"));
    map.insert(2, tr("Wydech"));
    map.insert(3, tr("Bezdech"));
    return map;
}
QList<EventInfo> SignalAnalyzer::getEvents() const
{
    return events;
}

QList<EventInfo> SignalAnalyzer::getEvents(qint16 type)
{
    QList<EventInfo> list;
    int i, size;

    if(!loaded) return list;

    size = events.size();
    for(i=0;i<size;i++)
    {
        if(events.at(i).type == type)
        {
            list << events.at(i);
        }
    }
    return list;
}




const QMap<qint16, QString> SignalAnalyzer::eventTypes = initEventType();





