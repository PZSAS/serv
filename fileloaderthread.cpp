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
    QFile file;
    QByteArray data;
    QDateTime startTime;
    QString fileName;
    int durationTime;

    dir.setPath(settings.value("dataDir").toString());
    fileList = dir.entryList(QStringList("*.fmd"));

    while(!fileList.isEmpty())
    {
        fileName = fileList.takeFirst();
        file.setFileName(dir.absoluteFilePath(fileName));
        if(!file.open(QIODevice::ReadOnly)) continue;
        data = file.read(18);
        file.close();
        startTime = QDateTime::fromTime_t(Container::byteToInt32(data.mid(4,4)));
        durationTime = Container::byteToInt32(data.mid(14,4));
        emit fileInfo(fileName, startTime, durationTime);

    }

}
