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
}

FilePreview::~FilePreview()
{
    delete ui;
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
