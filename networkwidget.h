#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>

namespace Ui {
class NetworkWidget;
}

class NetworkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkWidget(QWidget *parent = 0);
    ~NetworkWidget();

private:
    Ui::NetworkWidget *ui;
};

#endif // NETWORKWIDGET_H
