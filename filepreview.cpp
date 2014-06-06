#include "filepreview.h"
#include "ui_filepreview.h"

FilePreview::FilePreview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilePreview)
{
    ui->setupUi(this);

    updateInfo = true;
    lastX = -1;

    QSettings settings;
    ui->dataDirPathLabel->setText(settings.value("dataDir").toString());

    connect(ui->refreshListButton, SIGNAL(clicked()), ui->fileListTable, SLOT(loadFileList()));
    connect(ui->fileListTable, SIGNAL(cellClicked(int,int)), this, SLOT(previewFileChanged(int,int)));
    connect(ui->fileListTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(openAnalyzeWindow(int,int)));
}

FilePreview::~FilePreview()
{
    delete ui;
}

void FilePreview::showFileInfo(QString fileName, SignalAnalyzer *s)
{
    QMap<qint16, StatInfo> stats;
    QList<qint16> keys;
    QString info;
    int i, size;
    if(s == NULL || fileName.length() < 1)
    {
        ui->infoLabel->setText("");
        return;
    }
    info.append(tr("Nazwa pliku: "));
    info.append(fileName + "\n\n");

    stats = s->getStats();
    keys = stats.keys();
    size = keys.size();
    for(i=0;i<size;i++)
    {
        info += SignalAnalyzer::eventTypes.value(keys.at(i));
        info += ":\n- średni czas trwania: ";
        info += QString::number(stats.value(keys.at(i)).meanTime/1000.0);
        info += "s\n- wystąpień: ";
        info += QString::number(stats.value(keys.at(i)).count);
        info += "\n";

//        number = stats.value(keys.at(i)/1000.0);
//        tmp = QString("Średnia dla %1: %2s\n");
//        tmp = tmp.arg(SignalAnalyzer::eventTypes.value(keys.at(i)));
//        tmp = tmp.arg(QString::number(number));
//        qDebug() << tmp;
//        info.append(tmp);
    }

    ui->infoLabel->setText(info);
}

void FilePreview::openAnalyzeWindow(int x, int y)
{
    Q_UNUSED(y);
    if(ui->fileListTable->item(x, 2) == NULL) return;
    QString fileName = ui->fileListTable->item(x, 2)->text();


    // pozwol na gora 3 otwarte okna
    if(AnalyzeWindow::instancesCount() > 2) return;

    Container *c = new Container();
    c->loadFromFile(fileName);

    SignalAnalyzer *s = new SignalAnalyzer;
    s->loadFromFile(fileName.left(fileName.size()-4).append(".fad"));



    AnalyzeWindow *w = new AnalyzeWindow(c, s);
    w->show();
}




void FilePreview::on_changeDataDirLabel_clicked()
{
    QSettings settings;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Wybierz katalog"),
                                                     settings.value("dataDir").toString(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    if(dir.length() < 1) return;
    ui->dataDirPathLabel->setText(dir);
    settings.setValue("dataDir", dir);
    settings.sync();
    ui->fileListTable->loadFileList();
    updateInfo = true;
}

void FilePreview::previewFileChanged(int x, int y)
{
    Q_UNUSED(y);
    if(ui->fileListTable->item(x, 2) == NULL) return;
    QString fileName = ui->fileListTable->item(x, 2)->text();


    if(!updateInfo && (lastX == x)) return;
    lastX = x;
    updateInfo = false;


    if(AnalyzeWindow::instancesCount() > 2)
    {
        return;
    }

    SignalAnalyzer *s = new SignalAnalyzer;
    s->loadFromFile(fileName.left(fileName.size()-4).append(".fad"));
    showFileInfo(fileName, s);
    delete s;

//    Container *c = new Container();
//    SignalAnalyzer *s = new SignalAnalyzer;
//    c->loadFromFile(fileName);
//    s->loadFromFile(fileName.left(fileName.size()-4).append(".fad"));
//    showFileInfo(fileName, s);


//    AnalyzeWindow *w = new AnalyzeWindow(c, s);
//    w->show();
//    delete c;

}




