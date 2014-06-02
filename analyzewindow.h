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

class AnalyzeWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AnalyzeWindow(Container *data, SignalAnalyzer *infoData, QWidget *parent = 0);
    ~AnalyzeWindow();

protected:
    virtual void closeEvent (QCloseEvent * e );

private:
    void makeGUI();
    void updateEventWidget();

signals:

public slots:
    void changeEventType(int idx);

private:
    Container *data;
    SignalAnalyzer *infoData;
    QDockWidget *dockWidget;
    QComboBox *eventTypeWidget;
    QListWidget *eventListWidget;
    Plot *plot;
};

#endif // ANALYZEWINDOW_H
