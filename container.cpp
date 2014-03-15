#include "container.h"

Container::Container(QObject *parent) :
    QObject(parent)
{
}

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

bool Container::validateData(QByteArray data)
{
    QByteArray pattern;
    int i, tmp;
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

    // poprawic dla i>0
    for(i=0;i<liczbaSygnalow;i++)
    {
        pattern.setRawData("DATA",4);
        if(data.mid(26,4) != pattern)
        {
            qDebug() << "DATA" << i;
            return false;
        }
        id = byteToInt16(data.mid(30,2));
        czestotliwosc = byteToInt16(data.mid(32,2));
        rozmiarProbki = byteToInt16(data.mid(34,2));
        if(rozmiarProbki != 1 && rozmiarProbki != 2 && rozmiarProbki != 4)
        {
            qDebug() << "rozmiarProbki" << i;
            return false;
        }
        liczbaProbek = byteToInt32(data.mid(36,4));
        if(liczbaSekund != (rozmiarProbki*liczbaProbek/czestotliwosc))
        {
            qDebug() << "liczbaSekund" << i;
            qDebug() << "sekundy" << liczbaSekund << "r probki" << rozmiarProbki
                         << "l probek" << liczbaProbek  << "cz" << czestotliwosc;
            return false;
        }
        pattern.setRawData("STOP",4);
        tmp = 44 + liczbaProbek*rozmiarProbki;
        if(data.mid(tmp,4) != pattern)
        {
            qDebug() << "STOP" << i;
            return false;
        }

    }

    return true;
}






