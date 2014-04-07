#ifndef SIGNALANALYZER_H
#define SIGNALANALYZER_H

#include <QObject>

class SignalAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit SignalAnalyzer(QObject *parent = 0);

signals:

public slots:

};

#endif // SIGNALANALYZER_H
