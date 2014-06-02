#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>

#include "container.h"




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

protected:
    void paintEvent(QPaintEvent *);


    QDateTime startTime;
    qint32 durationTime;
    SampleInfo samplesInfo;
    QVector<qint16> samples;
    int vLines;
    bool loaded;



};

#endif // PLOT_H
