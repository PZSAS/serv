#include "plot.h"

Plot::Plot(QWidget *parent) :
    QWidget(parent)
{
    loaded = false;
    vLines = 8;
    setMinimumSize(200, 200);
    setMaximumHeight(200);
    setCursor(Qt::SizeHorCursor);
}

Plot::~Plot()
{
}

bool Plot::setData(Container *data, qint16 idx)
{
    int i, size;
    if(data == NULL) return false;
    if(!data->contains(idx))
    {
        qDebug() << "Nie istnieje sygnal o podanym identyfikatorze";
        qDebug() << data->getSamplesInfo().keys();
        return false;
    }
    samples = data->getSamplesFromIndex(idx);
    samplesInfo = data->getSamplesInfoFromIndex(idx);
    durationTime = data->getDurationTime();
    startTime = data->getStartTime();
    if(durationTime < 1) return false;

    // szukaj minimum i maksimum
    size = samples.size();
    if(size < 1)
    {
        return false;
    }
    minVal = samples[0];
    maxVal = samples[0];
    for(i=0;i<size;i++)
    {
        if(samples[i] < minVal) minVal = samples[i];
        else if(samples[i] > maxVal) maxVal = samples[i];
    }


    loaded = true;
    return true;
}

bool Plot::setData(QVector<qint16> data, int durationTime)
{
    int i, size;
    size = data.size();
    if(size < 1) return false;
    this->durationTime = durationTime;
    samples = data;
    minVal = samples[0];
    maxVal = samples[0];
    for(i=0;i<size;i++)
    {
        if(samples[i] < minVal) minVal = samples[i];
        else if(samples[i] > maxVal) maxVal = samples[i];
    }

    // przesun wykres na dol i daj odstep 4px
    for(i=0;i<size;i++)
    {
        samples[i] -= minVal - 4;
    }

    loaded = true;
    return true;
}

void Plot::setEvents(QList<EventInfo> events)
{
    this->events = events;
    repaint();
}

void Plot::paintEvent(QPaintEvent *)
{

    if(!loaded) return;
    QColor color;
    int w = geometry().width();
    int h = geometry().height();
    int max = samples.count();
    int i, size;
    int hTmp;
    float step = (float)w/(float)max;
    float eS, eD, dF;
    QPen pen;

    QPainter painter;
    painter.begin(this);

    // biale tlo
    painter.fillRect(QRect(QPoint(0,0), geometry().size()), Qt::white);

    // siatka
    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    hTmp = float (h)/vLines;
    for(i=1;i<vLines;i++)
    {
            painter.drawLine(0, i*hTmp, w, i*hTmp);
    }


    painter.setPen(Qt::SolidLine);
    painter.setPen(Qt::blue);

    //painter.drawRect(0, 0, w-1, h-1);

    max--;
    for(i=0; i<max; i++)
    {
        painter.drawLine(i*step, h-samples[i], (i+1)*step, h-samples[i+1]);
    }

    size = events.size();
    if(size > 0)
    {
        dF = durationTime*1000.0;
        for(i=0;i<size;i++)
        {
            color = QColor(255,0,0,100);
            if(events.at(i).type == 2) color = QColor(0,255,0,100);
            eS = events.at(i).startTime/dF;
            eD = eS + (events.at(i).durationTime/dF);
            painter.fillRect(QRectF(QPointF(eS*w, 0), QPointF(eD*w, h)),color);
        }
    }


    painter.end();
}





