#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QMap>

typedef struct
{

    qint16 freq;
    qint16 sampleSize;

} SampleInfo;

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
    static QVector<qint16> dataToInt16Vec(QByteArray &data);
    static QVector<qint16> data8ToInt16Vec(QByteArray &data);
    static bool validateData(QByteArray data);
    static Container* getCurrent();

public slots:

public:
    bool load(QByteArray data, bool append = false);
    void clear();
    bool twoByte(QVector<qint16> tab);

private:
    static Container *current;
    QDateTime startTime;
    int durationTime;
    QMap<qint16, SampleInfo> samplesInfo;
    QMap<qint16, QVector<qint16>> samples;
    bool loaded;
};

#endif // CONTAINER_H
