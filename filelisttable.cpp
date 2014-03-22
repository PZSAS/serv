#include "filelisttable.h"

FileListTable::FileListTable(QWidget *parent) :
    QTableWidget(parent)
{
    setColumnCount(3);
    setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Data zapisu")));
    setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Czas trwania")));
    setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Plik")));
    setSelectionBehavior(QAbstractItemView::SelectRows);
    insertData("plik.fmd", QDateTime(QDate(2014,3,21), QTime(6,33,45)), 8000);
    insertData("plik.fmd", QDateTime(QDate(2014,3,21), QTime(6,33,45)), 8000);
    insertData("plik.fmd", QDateTime(QDate(2014,3,21), QTime(6,33,45)), 8000);


    loadFileList();
}


void FileListTable::insertData(QString fileName, QDateTime startTime, int duration)
{
    this->setRowCount(this->rowCount()+1);
    QTableWidgetItem* item = new QTableWidgetItem(startTime.toString("dd.MM.yyyy hh:mm"));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    this->setItem(this->rowCount()-1, 0, item);
    QTime time = QTime(0,0,0).addSecs(duration);
    item = new QTableWidgetItem(time.toString("hh:mm:ss"));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    this->setItem(this->rowCount()-1, 1, item);
    item = new QTableWidgetItem(fileName);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    this->setItem(this->rowCount()-1, 2, item);


}

void FileListTable::loadFileList()
{
    thread = new FileLoaderThread();
    connect(thread, SIGNAL(fileInfo(QString,QDateTime,int)), this, SLOT(insertData(QString,QDateTime,int)));
    thread->start();
    thread->deleteLater();


}



