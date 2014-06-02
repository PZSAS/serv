#include "analyzewindow.h"

AnalyzeWindow::AnalyzeWindow(Container *data, SignalAnalyzer *infoData, QWidget *parent) :
    QMainWindow(parent)
{
    this->data = data;
    this->infoData = infoData;
    makeGUI();
    resize(800, 600);
    updateEventWidget();
}

AnalyzeWindow::~AnalyzeWindow()
{

}

void AnalyzeWindow::closeEvent(QCloseEvent *e)
{
    e->accept();
    this->deleteLater();
}

void AnalyzeWindow::makeGUI()
{
    // nie dziala, naprawic
    QHBoxLayout *mainLayout = new QHBoxLayout;
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

    plot = new Plot();
    plot->setData(data, 3);
    mainLayout->addWidget(plot);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void AnalyzeWindow::updateEventWidget()
{
    QList<qint16> eventTypes;
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
}



