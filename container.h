#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QDebug>

class Container : public QObject
{
    Q_OBJECT
public:
    explicit Container(QObject *parent = 0);

signals:

public:
    static qint32 byteToInt32(QByteArray &data);
    static qint16 byteToInt16(QByteArray &data);
    static quint8 byteToUInt8(QByteArray &data);
    static bool validateData(QByteArray data);

public slots:

};

#endif // CONTAINER_H
