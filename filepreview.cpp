#include "filepreview.h"
#include "ui_filepreview.h"

FilePreview::FilePreview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilePreview)
{
    ui->setupUi(this);

    QSettings settings;
    ui->dataDirPathLabel->setText(settings.value("dataDir").toString());

    connect(ui->refreshListButton, SIGNAL(clicked()), ui->fileListTable, SLOT(loadFileList()));
    connect(ui->fileListTable, SIGNAL(cellPressed(int,int)), this, SLOT(previewFileChanged(int,int)));
}

FilePreview::~FilePreview()
{
    delete ui;
}

void FilePreview::showFileInfo(QString fileName)
{
    if(fileName.length() < 1)
    {
        ui->infoLabel->setText("");
        return;
    }
    QString info;
    info.append(tr("Nazwa pliku: "));
    info.append(fileName + "\n");
    info.append(tr("Liczba bezdechów: "));
    info.append("\n");
    info.append(tr("Średni czas bezdechów: "));
    info.append("\n");
    info.append(tr("Średni czas wdechów: "));
    info.append("\n");
    info.append(tr("Średni czas wydechów: "));
    info.append("\n");
    ui->infoLabel->setText(info);
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
}

void FilePreview::previewFileChanged(int x, int y)
{
    Q_UNUSED(y);
    if(ui->fileListTable->item(x, 2) == NULL) return;
    QString fileName = ui->fileListTable->item(x, 2)->text();

    showFileInfo(fileName);
    return;
    Container *c = new Container();
    SignalAnalyzer *s = new SignalAnalyzer;
    c->loadFromFile(fileName);
    s->loadFromFile(fileName.left(fileName.size()-4).append(".fad"));
    AnalyzeWindow *w = new AnalyzeWindow(c, s);
    w->show();
    delete c;

}




