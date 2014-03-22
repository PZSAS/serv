#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QSettings>

typedef struct
{

    qint16 freq;
    qint16 sampleSize;

} SampleInfo;

typedef struct
{

    QDateTime startDate;
    int duration;
    int signalsCount;

} FileInfo;

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
    static QByteArray int32ToByte(qint32 number);
    static QByteArray int16ToByte(qint16 number);
    static QByteArray uint8ToByte(quint8 number);
    static QByteArray int16VecToByte8(QVector<qint16> tab);
    static QByteArray int16VecToByte16(QVector<qint16> tab);
    static QVector<qint16> dataToInt16Vec(QByteArray &data);
    static QVector<qint16> data8ToInt16Vec(QByteArray &data);
    static bool validateData(QByteArray data);
    static FileInfo fileInfo(QString fileName, bool absolutePath = false);
    static Container* getCurrent();

public slots:

public:
    bool load(QByteArray data, bool append = false);
    void clear();
    bool saveToFile(QString fileName = QString());
    int getDurationTime();
    QDateTime getStartTime();

private:
    static Container *current;
    QDateTime startTime;
    qint32 durationTime;
    QMap<qint16, SampleInfo> samplesInfo;
    QMap<qint16, QVector<qint16>> samples;
    bool loaded;
};

#endif // CONTAINER_H
