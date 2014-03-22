#ifndef FILEPREVIEW_H
#define FILEPREVIEW_H

#include <QMainWindow>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>

namespace Ui {
class FilePreview;
}

class FilePreview : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilePreview(QWidget *parent = 0);
    ~FilePreview();

private slots:

    void on_changeDataDirLabel_clicked();

private:
    Ui::FilePreview *ui;
};

#endif // FILEPREVIEW_H
