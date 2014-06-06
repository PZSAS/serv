#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>

#include "container.h"
#include "signalanalyzer.h"




class Plot : public QWidget
{
    Q_OBJECT
public:
    explicit Plot(QWidget *parent = 0);
    ~Plot();

signals:

public slots:

public:
    bool setData(Container *data, qint16 idx);
    bool setData(QVector<qint16> data, int durationTime);
    void setEvents(QList<EventInfo> events);

protected:
    void paintEvent(QPaintEvent *);

    QList<EventInfo> events;
    QDateTime startTime;
    qint32 durationTime;
    SampleInfo samplesInfo;
    QVector<qint16> samples;
    qint32 minVal, maxVal;
    int vLines;
    bool loaded;



};

#endif // PLOT_H
