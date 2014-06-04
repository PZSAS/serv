#ifndef ANALYZEWINDOW_H
#define ANALYZEWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QComboBox>
#include <QListWidget>
#include <QTime>
#include "container.h"
#include "signalanalyzer.h"
#include "plot.h"

#define MAX_PLOT_COUNT 5

class AnalyzeWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AnalyzeWindow(Container *data, SignalAnalyzer *infoData, QWidget *parent = 0);
    ~AnalyzeWindow();
    void close();
    static int instancesCount();

protected:
    virtual void closeEvent (QCloseEvent * e );

private:
    void makeGUI();
    void updateEventWidget();

signals:

public slots:
    void changeEventType(int idx);

private:
    void setPlotCount(int count);

    Container *data;
    SignalAnalyzer *infoData;
    QDockWidget *dockWidget;
    QComboBox *eventTypeWidget;
    QListWidget *eventListWidget;
    Plot *plot[MAX_PLOT_COUNT];
    int plotCount;
    static int instances;
};

#endif // ANALYZEWINDOW_H
