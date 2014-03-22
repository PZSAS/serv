#include "fileloaderthread.h"

FileLoaderThread::FileLoaderThread(QObject *parent) :
    QThread(parent)
{
}

void FileLoaderThread::run()
{
    QSettings settings;
    QDir dir;
    QStringList fileList;
    QString fileName;
    FileInfo fi;

    dir.setPath(settings.value("dataDir").toString());
    fileList = dir.entryList(QStringList("*.fmd"), QDir::NoFilter, QDir::Time);

    while(!fileList.isEmpty())
    {
        fileName = fileList.takeFirst();
        fi = Container::fileInfo(fileName);
        emit fileInfo(fileName, fi.startDate, fi.duration);

    }
}
