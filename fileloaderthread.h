#ifndef FILELOADERTHREAD_H
#define FILELOADERTHREAD_H

#include <QThread>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDateTime>

#include "container.h"

class FileLoaderThread : public QThread
{
    Q_OBJECT
public:
    explicit FileLoaderThread(QObject *parent = 0);

signals:

    void fileInfo(QString, QDateTime, int);

public slots:

private:
    void run();

};

#endif // FILELOADERTHREAD_H
