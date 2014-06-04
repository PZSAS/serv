#include "plot.h"

Plot::Plot(QWidget *parent) :
    QWidget(parent)
{
    loaded = false;
    vLines = 8;
    setMinimumSize(300, 200);
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

bool Plot::setData(QVector<qint16> data)
{
    int i, size;
    size = data.size();
    if(size < 1) return false;
    samples = data;
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

void Plot::paintEvent(QPaintEvent *)
{

    if(!loaded) return;
    int w = geometry().width();
    int h = geometry().height();
    int max = samples.count();
    int i;
    int hTmp;
    float step = (float)w/(float)max;
    QPen pen;

    QPainter painter;
    painter.begin(this);

    // biale tlo
    painter.fillRect(QRect(QPoint(0,0), geometry().size()), Qt::white);
    painter.fillRect(QRect(QPoint(), QPoint(20, h)), Qt::gray);

    // siatka
    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    hTmp = float (h)/vLines;
    for(i=1;i<vLines;i++)
    {
            painter.drawLine(0, i*hTmp, w, i*hTmp);
    }
    // nie robi w jednej petli zeby
    // nie przelaczac co chwile pedzla
    painter.setPen(Qt::black);
    for(i=1;i<vLines;i++)
    {
            painter.drawLine(0, i*hTmp, 20, i*hTmp);
    }

    //painter.setRenderHint(QPainter::Antialiasing);


    painter.setPen(Qt::SolidLine);
    painter.setPen(Qt::blue);

    //painter.drawRect(0, 0, w-1, h-1);

    max--;
    for(i=0; i<max; i++)
    {
        painter.drawLine(i*step, h-samples[i], (i+1)*step, h-samples[i+1]);
    }



    painter.end();
    QByteArray zz;
}





