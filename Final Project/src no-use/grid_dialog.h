#ifndef GRID_DIALOG_H
#define GRID_DIALOG_H

#include <QDialog>
#include <QImage>
#include <QBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QDebug>
#include <mylabel.h>

namespace Ui {
class grid_dialog;
}

class grid_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit grid_dialog(QWidget *parent = 0);
    ~grid_dialog();
    int*data;

private:
    Ui::grid_dialog *ui;
    MyLabel *m_label;
    QVBoxLayout *mainLayout;
    QHBoxLayout *bottomlayout;
    QPushButton *OK;
    QPushButton *Cancle;



public slots:
    void on_ok_clicked();
    void on_cancel_clicked();
};

#endif // GRID_DIALOG_H
