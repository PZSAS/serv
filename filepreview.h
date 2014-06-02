#ifndef FILEPREVIEW_H
#define FILEPREVIEW_H

#include <QMainWindow>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include "analyzewindow.h"

namespace Ui {
class FilePreview;
}

class FilePreview : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilePreview(QWidget *parent = 0);
    ~FilePreview();

private:
    void showFileInfo(QString fileName = QString());

private slots:

    void on_changeDataDirLabel_clicked();
    void previewFileChanged(int x, int y);

private:
    Ui::FilePreview *ui;
};

#endif // FILEPREVIEW_H
