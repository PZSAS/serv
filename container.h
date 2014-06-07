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
    void currentUpdated();

public:
    static qint32 byteToInt32(QByteArray &data);
    static qint16 byteToInt16(QByteArray &data);
    static quint8 byteToUInt8(QByteArray &data);
    static QByteArray int32ToByteBE(qint32 number);
    static QByteArray int32ToByte(qint32 number);
    static QByteArray int16ToByte(qint16 number);
    static QByteArray uint8ToByte(quint8 number);
    static QByteArray int16VecToByte8(QVector<qint16> tab);
    static QByteArray int16VecToByte16(QVector<qint16> tab);
    static QVector<qint16> dataToInt16Vec(QByteArray &data);
    static QVector<qint16> data8ToInt16Vec(QByteArray &data);
    static QVector<quint8> data8ToInt8Vec(QByteArray &data);

    static bool validateData(QByteArray data);
    static FileInfo fileInfo(QString fileName, bool absolutePath = false);
    static Container* getCurrent();
    QMap<qint16, QVector<qint16> > getSamples();
    QMap<qint16, SampleInfo> getSamplesInfo();
    QVector<qint16> getSamplesFromIndex(qint16 idx);
    SampleInfo getSamplesInfoFromIndex(qint16 idx);
    bool contains(qint16 idx);

public slots:

public:
    bool load(QByteArray &data, bool append = false);
    void loadHeaders(qint16 channel, SampleInfo info);
    bool loadFromFile(QString fileName, bool absolutePath = false);
    void addSamples(QVector<quint8> sig);
    void clear();
    bool saveToFile(QString fileName = QString());
    int getDurationTime();
    QDateTime getStartTime();
    bool isLoaded();

private:
    static Container *current;
    QDateTime startTime;
    qint32 durationTime;
    QMap<qint16, SampleInfo> samplesInfo;
    QMap<qint16, QVector<qint16> > samples;
    quint16 lastIterator;
    bool loaded;
};

#endif // CONTAINER_H
