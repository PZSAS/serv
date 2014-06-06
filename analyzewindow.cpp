#include "analyzewindow.h"

AnalyzeWindow::AnalyzeWindow(Container *data, SignalAnalyzer *infoData, QWidget *parent) :
    QMainWindow(parent)
{
    instances++;
    this->data = data;
    this->infoData = infoData;


    makeGUI();
    setMinimumSize(700, 400);
    resize(800, 600);
    updateEventWidget();
}

AnalyzeWindow::~AnalyzeWindow()
{
    instances--;
}

void AnalyzeWindow::close()
{
    closeEvent(new QCloseEvent());
}

int AnalyzeWindow::instancesCount()
{
    return instances;
}

void AnalyzeWindow::closeEvent(QCloseEvent *e)
{
    e->accept();
    delete infoData;
    delete data;
    this->deleteLater();
}

void AnalyzeWindow::makeGUI()
{
    int i;
    QList<qint16> signalsIdx;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QWidget *mainWidget = new QWidget;

    eventTypeWidget = new QComboBox;
    eventListWidget = new QListWidget;
    QVBoxLayout *dockLayout = new QVBoxLayout;
    dockLayout->addWidget(eventTypeWidget);
    dockLayout->addWidget(eventListWidget);
    QWidget *dockLayoutWidget = new QWidget;
    dockLayoutWidget->setLayout(dockLayout);

    dockWidget = new QDockWidget(tr("Zdarzenia"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockWidget->setWidget(dockLayoutWidget);
    dockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    dockWidget->setSizeIncrement(0,0);


    signalsIdx = data->getSamplesInfo().keys();
    plotCount = signalsIdx.size();
    for(i=0;i<plotCount;i++)
    {
        if(i>MAX_PLOT_COUNT) break;
        plot[i] = new Plot();
        plot[i]->setData(data->getSamplesFromIndex(signalsIdx.value(i)), data->getDurationTime());
        //plot[i]->setData(data, signalsIdx.value(i));
        mainLayout->addWidget(plot[i]);
    }
    //tymczasowo
    if(plotCount<2)
    {
        mainLayout->addItem( new QSpacerItem(40, 300, QSizePolicy::Expanding, QSizePolicy::Minimum));
    }

//    plot[0] = new Plot();
//    plot[0]->setData(data, 3);
//    mainLayout->addWidget(plot);



    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void AnalyzeWindow::updateEventWidget()
{
    QList<EventInfo> events;
    QString val;
    QTime time;
    int count, i;

    eventTypeWidget->disconnect();
    eventTypeWidget->clear();
    eventListWidget->clear();
    eventListWidget->disconnect();

    eventTypeWidget->addItem(tr("Wszystkie"));
    eventTypeWidget->addItems(infoData->listOfTypes());

    events = infoData->getEvents();
    count = events.size();
    for(i=0;i<count;i++)
    {
        time = QTime(0,0,0,0); // zerowanie czasu
        time = time.addMSecs(events.value(i).startTime);
        val = time.toString("hh:mm:ss") + " (" +
                QString::number(events.value(i).durationTime/1000.0) + ")";
        eventListWidget->addItem(val);
    }
    for(i=0;i<plotCount;i++)
    {
        if(i>MAX_PLOT_COUNT) break;
        plot[i]->setEvents(events);
    }
    connect(eventTypeWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(changeEventType(int)));
}

void AnalyzeWindow::changeEventType(int idx)
{
    int i, count;
    qint16 type;
    QList<EventInfo> events;
    QList<qint16> eventTypes;
    QTime time;
    QString val;

    eventTypes << infoData->indexesOfTypes();

    if(idx == 0)
    {
        events = infoData->getEvents();
    }
    else
    {
        --idx;
        if(idx>eventTypes.size()) return;
        type = eventTypes.value(idx);
        events = infoData->getEvents(type);
    }
    eventListWidget->clear();
    eventListWidget->disconnect();
    count = events.size();
    for(i=0;i<count;i++)
    {
        time = QTime(0,0,0,0); // zerowanie czasu
        time = time.addMSecs(events.value(i).startTime);
        val = time.toString("hh:mm:ss") + " (" +
                QString::number(events.value(i).durationTime/1000.0) + ")";
        eventListWidget->addItem(val);
    }
    for(i=0;i<plotCount;i++)
    {
        if(i>MAX_PLOT_COUNT) break;
        plot[i]->setEvents(events);
    }
}

void AnalyzeWindow::setPlotCount(int count)
{
    if(count < 0) plotCount = 0;
    else if(count > MAX_PLOT_COUNT) plotCount = MAX_PLOT_COUNT;
    else plotCount = count;
}

int AnalyzeWindow::instances=0;

