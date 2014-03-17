#include "networkthread.h"

NetworkThread::NetworkThread(QObject *parent) :
    QThread(parent)
{
    password = QString();
    ready = false;
}

NetworkThread::~NetworkThread()
{
}

void NetworkThread::setSocket(int socketDesc)
{
    socketDescriptor = socketDesc;
    ready = true;
}

void NetworkThread::setPassword(QString pass)
{
    password = pass;
}

void NetworkThread::run()
{
    if(!ready) return;
    QTcpSocket socket;
    QByteArray start;
    QByteArray request;
    QByteArray buffer;
    QStringList headers;
    QString pass;

    if (!socket.setSocketDescriptor(socketDescriptor))
    {
        qDebug() << "problem z socketem";
        return;
    }

    int pos, i=0, bytes, max;
    start.setRawData("\r\n\r\n",4);
    if(true)
    {
        // get request
        while(i<40)
        {
            i++;
            if(socket.state() != QAbstractSocket::ConnectedState)
            {
                socket.close();
                return;
            }
            if(socket.bytesAvailable() < 1)
            {
                if(!socket.isOpen()) return;
                socket.waitForReadyRead(500);
                continue;
            }

            request.append(socket.readAll());
            if(request.size() > 2000)
            {

                socket.write(generateHeaders(400));
                socket.waitForBytesWritten(2000);
                socket.write(infoHTML("Bad Request", "Request too long"));
                socket.waitForBytesWritten(2000);
                socket.close();
                return;
            }
            // jezeli znalazl koniec to przestaje czytac
            if(request.indexOf(start) >= 0) break;
        }

        // analize request
        QTextStream in(&request, QIODevice::ReadOnly);
        QString method;
        QString fileName;

        for(i=0;i<20;i++)
        {
            headers.append(in.readLine());
            if(in.atEnd()) break;
        }
        pos = headers.value(0).indexOf(" ");
        if(pos < 1)
        {
            socket.write(generateHeaders(400));
            socket.waitForBytesWritten(2000);
            socket.write(infoHTML("Bad Request", "400 Bad Request"));
            socket.waitForBytesWritten(2000);
            socket.close();
            exit();
            return;
        }
        method = headers.value(0).left(pos);
        fileName = headers.value(0).mid(pos+1);
        pos = fileName.indexOf(" ");
        fileName = fileName.left(pos);
        fileName.remove("/");
        fileName.remove("\\");
        pos = fileName.indexOf("?");
        if(pos > 0) fileName = fileName.left(pos);

        if(password.length() > 0)
        {
            pass = QString();
            max = headers.length();
            for(i=0;i<max;i++)
            {
                if(headers.value(i).left(6) == "Pass: ")
                {
                    pass = headers.value(i).mid(6, 32);
                }
            }
            buffer = password.toLocal8Bit();
            qDebug() << buffer;
            password = QString(QCryptographicHash::hash(buffer,QCryptographicHash::Md5).toHex());
            if(pass != password)
            {
                socket.write(generateHeaders(403));
                socket.waitForBytesWritten(2000);
                socket.write(infoHTML("Forbidden", "zle haslo"));
                socket.waitForBytesWritten(2000);
                socket.close();
                return;
            }
        }


        if(socket.state() != QAbstractSocket::ConnectedState)
        {
            socket.close();
            return;
        }
        if(fileName.length() < 1)
        {
            socket.write(generateHeaders(404));
            socket.waitForBytesWritten(2000);
            socket.write(infoHTML("Not Found", "404: Nie ma takiego pliku"));
            socket.waitForBytesWritten(2000);
            socket.close();
            return;
        }
        if(fileName == "lista.xml")
        {
            socket.write(generateHeaders(200, "text/xml"));
            socket.waitForBytesWritten(2000);
            socket.write(fileXmlList());
            socket.waitForBytesWritten(2000);
            socket.close();
            return;
        }
        if(fileName == "lista.txt")
        {
            socket.write(generateHeaders(200, "text/plain"));
            socket.waitForBytesWritten(2000);
            socket.write(fileTxtList());
            socket.waitForBytesWritten(2000);
            socket.close();
            return;
        }
        QDir dir;
        QFile file;
        QSettings settings;
        dir.setPath(settings.value("dataDir").toString());
        file.setFileName(dir.absoluteFilePath(fileName));
        if(!dir.exists() || !file.exists() || !file.open(QIODevice::ReadOnly))
        {
            socket.write(generateHeaders(404));
            socket.waitForBytesWritten(2000);
            socket.write(infoHTML("Not Found", "404: Nie ma takiego pliku"));
            socket.waitForBytesWritten(2000);
            socket.close();
            if(file.isOpen()) file.close();
            return;
        }
        socket.write(generateHeaders(200, "application/octet-stream"));
        socket.waitForBytesWritten(2000);
        while(!file.atEnd())
        {
            buffer = file.read(4096);
            bytes = socket.write(fileTxtList());
            if(bytes < 0) // if error
            {
                socket.close();
                file.close();
                return;
            }
            socket.waitForBytesWritten(2000);
        }
        file.close();
        socket.close();
    }
}

void NetworkThread::doStuff()
{
    QTcpSocket *socket;
    if(socket == NULL)
    {
        return;
    }
    QByteArray start;
    QByteArray request;
    QByteArray buffer;
    QStringList headers;
    QString pass;
    int pos, i=0, bytes, max;
    start.setRawData("\r\n\r\n",4);
    if(true)
    {
        // get request
        while(i<40)
        {
            if(socket->bytesAvailable() < 1)
            {
                if(!socket->isOpen()) return;
                socket->waitForReadyRead(500);
                continue;
            }
            request.append(socket->readAll());
            if(request.size() > 2000)
            {
                socket->write(generateHeaders(400));
                socket->waitForBytesWritten(2000);
                socket->write(infoHTML("Bad Request", "Request too long"));
                socket->waitForBytesWritten(2000);
                socket->close();
                exit();
                return;
            }
            // jezeli znalazl koniec to przestaje czytac
            if(request.indexOf(start) >= 0) break;
            i++;
        }

        // analize request
        QTextStream in(&request, QIODevice::ReadOnly);
        QString method;
        QString fileName;

        for(i=0;i<20;i++)
        {
            headers.append(in.readLine());
            if(in.atEnd()) break;
        }
        pos = headers.value(0).indexOf(" ");
        if(pos < 1)
        {
            socket->write(generateHeaders(400));
            socket->waitForBytesWritten(2000);
            socket->write(infoHTML("Bad Request", "400 Bad Request"));
            socket->waitForBytesWritten(2000);
            socket->close();
            exit();
            return;
        }
        method = headers.value(0).left(pos);
        fileName = headers.value(0).mid(pos+1);
        pos = fileName.indexOf(" ");
        fileName = fileName.left(pos);
        fileName.remove("/");
        fileName.remove("\\");
        pos = fileName.indexOf("?");
        if(pos > 0) fileName = fileName.left(pos);

        if(password.length() > 0)
        {
            // to nie dziala
            // telnet tez nie diala
            // sie zawiesza czasami
            // nie wiem o co chodzi
            // :(
            pass = QString();
            max = headers.length();
            for(i=0;i<max;i++)
            {
                if(headers.value(i).left(6) == "Pass: ")
                {
                    pass = headers.value(i).mid(6, 32);
                }
            }
            buffer = password.toLocal8Bit();
            qDebug() << buffer;
            password = QString(QCryptographicHash::hash(buffer,QCryptographicHash::Md5).toHex());
            qDebug() << pass << password << headers;
            if(pass != password)
            {
                socket->write(generateHeaders(403));
                socket->waitForBytesWritten(2000);
                socket->write(infoHTML("Forbidden", "zle haslo"));
                socket->waitForBytesWritten(2000);
                socket->close();
                exit();
                return;
            }
        }


        if(fileName.length() < 1)
        {
            socket->write(generateHeaders(404));
            socket->waitForBytesWritten(2000);
            socket->write(infoHTML("Not Found", "404: Nie ma takiego pliku"));
            socket->waitForBytesWritten(2000);
            socket->close();
            exit();
            return;
        }
        if(fileName == "lista.xml")
        {
            socket->write(generateHeaders(200, "text/xml"));
            socket->waitForBytesWritten(2000);
            socket->write(fileXmlList());
            socket->waitForBytesWritten(2000);
            socket->close();
            exit();
            return;
        }
        if(fileName == "lista.txt")
        {
            socket->write(generateHeaders(200, "text/plain"));
            socket->waitForBytesWritten(2000);
            socket->write(fileTxtList());
            socket->waitForBytesWritten(2000);
            socket->close();
            exit();
            return;
        }
        QDir dir;
        QFile file;
        QSettings settings;
        dir.setPath(settings.value("dataDir").toString());
        file.setFileName(dir.absoluteFilePath(fileName));
        if(!dir.exists() || !file.exists() || !file.open(QIODevice::ReadOnly))
        {
            socket->write(generateHeaders(404));
            socket->waitForBytesWritten(2000);
            socket->write(infoHTML("Not Found", "404: Nie ma takiego pliku"));
            socket->waitForBytesWritten(2000);
            socket->close();
            if(file.isOpen()) file.close();
            exit();
            return;
        }
        socket->write(generateHeaders(200, "application/octet-stream"));
        socket->waitForBytesWritten(2000);
        while(!file.atEnd())
        {
            buffer = file.read(4096);
            bytes = socket->write(fileTxtList());
            if(bytes < 0) // if error
            {
                socket->close();
                file.close();
                exit();
                return;
            }
            socket->waitForBytesWritten(2000);
        }
        file.close();
        socket->close();
        exit();
    }
}

QByteArray NetworkThread::generateHeaders(int code, QString mime)
{
    QString headers("HTTP/1.1 ");
    if(mime.length()<1) mime = QString("text/html");

    switch(code)
    {
    case 404:
        headers.append("404 Not Found\r\n");
        break;
    case 403:
        headers.append("403 Forbidden\r\n");
        break;
    case 400:
        headers.append("400 Bad Request\r\n");
        break;
    default:
        headers.append("200 OK\r\n");
        break;
    }
    headers.append("Connection: close\r\n");
    headers.append("Content-Type: ").append(mime).append("\r\n");

    headers.append("\r\n");
    return headers.toUtf8();
}

QByteArray NetworkThread::fileTxtList()
{
    QByteArray data;
    QSettings settings;
    QDir dir;
    QStringList list;
    dir.setPath(settings.value("dataDir").toString());
    if(!dir.exists())
    {
        qDebug() << "datalog danych nie istnieje";
        return data;
    }
    dir.setNameFilters(QStringList("*.fmd"));
    list = dir.entryList();
    while(list.size() > 0)
    {
        if(data.length() > 0) data.append("\r\n");
        data.append(list.takeFirst().toUtf8());
    }
    return data;
}

QByteArray NetworkThread::fileXmlList()
{
    QByteArray data;
    QSettings settings;
    QDir dir;
    QStringList list;
    dir.setPath(settings.value("dataDir").toString());
    if(!dir.exists())
    {
        qDebug() << "datalog danych nie istnieje";
        return data;
    }
    dir.setNameFilters(QStringList("*.fmd"));
    list = dir.entryList();
    data.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n<FPDM>\r\n\t<files>\r\n");
    while(list.size() > 0)
    {
        data.append("\t\t<file>");
        data.append(list.takeFirst().toUtf8());
        data.append("</file>\r\n");
    }
    data.append("\t</files>\r\n</FPDM>\r\n");
    return data;
}

QByteArray NetworkThread::infoHTML(QString title, QString info)
{
    QByteArray data;
    data.append("<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" >");
    data.append("\r\n<title>");
    data.append(title.toUtf8());
    data.append("</title>\r\n</head>\r\n<body>\r\n<p>");
    data.append(info);
    data.append("</p>\r\n</body>\r\n</html>\r\n");
    return data;
}



