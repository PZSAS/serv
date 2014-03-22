#ifndef FILELISTTABLE_H
#define FILELISTTABLE_H

#include <QTableWidget>
#include <QDateTime>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QDebug>

#include "container.h"
#include "fileloaderthread.h"

class FileListTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit FileListTable(QWidget *parent = 0);

signals:

private:
    static FileListTable *pointer;

public:


public slots:

    void loadFileList();

private slots:
    void insertData(QString fileName, QDateTime startTime, int duration);

private:
    FileLoaderThread *thread;

};

#endif // FILELISTTABLE_H
