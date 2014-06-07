#ifndef SIGNALANALYZER_H
#define SIGNALANALYZER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include "container.h"

typedef struct {
    int count;
    int meanTime;
} StatInfo;

typedef struct {
    qint16 type;
    int startTime;
    int durationTime;
} EventInfo;

class SignalAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit SignalAnalyzer(QObject *parent = 0);

    bool test();
    bool loadFromFile(QString fileName, bool absolutePath = false);
signals:


public:
    static bool isSignalKnown(qint16 idx);
    static bool isEventKnown(qint16 idx);
    QStringList listOfTypes();
    static QStringList listOfSignals();
    QList<qint16> indexesOfTypes();

public:
    static const QMap<qint16, QString> signalTypes;
    static const QMap<qint16, QString> eventTypes;



    QList<EventInfo> getEvents() const;
    QList<EventInfo> getEvents(qint16 type);

    QMap<qint16, StatInfo> getStats() const;

private:
    static QMap<qint16, QString> initSignalType();
    static QMap<qint16, QString> initEventType();
    bool loaded;
    QDateTime startTime;
    QMap<qint16, StatInfo> stats;
    QList<EventInfo> events;


public slots:

};

#endif // SIGNALANALYZER_H
