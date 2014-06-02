#include "plot.h"

Plot::Plot(QWidget *parent) :
    QWidget(parent)
{
    loaded = false;
    vLines = 8;
}

Plot::~Plot()
{
}

bool Plot::setData(Container *data, qint16 idx)
{
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
    if(samples.isEmpty()) return false;
    if(durationTime < 1) return false;
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

    // siatka
    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    hTmp = float (h)/vLines;
    for(i=1;i<vLines;i++)
    {
            painter.drawLine(0, i*hTmp, w, i*hTmp);
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





