#ifndef FILEPREVIEW_H
#define FILEPREVIEW_H

#include <QMainWindow>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include "analyzewindow.h"

namespace Ui {
class FilePreview;
}

class FilePreview : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilePreview(QWidget *parent = 0);
    void closeAllAnalyzeWindows();
    ~FilePreview();

private:
    void showFileInfo(QString fileName = QString(), SignalAnalyzer *s = NULL);

private slots:

    void on_changeDataDirLabel_clicked();
    void previewFileChanged(int x, int y);
    void openAnalyzeWindow(int x, int y);

private:
    Ui::FilePreview *ui;
    bool updateInfo;
    int lastX;
};

#endif // FILEPREVIEW_H
