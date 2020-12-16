#ifndef ROUTE_H
#define ROUTE_H

#include <QDialog>
#include <QLayout>
#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QVector>
#include <datainfo.h>
namespace Ui {
class route;
}

class route : public QDialog
{
    Q_OBJECT
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
public:
    explicit route(QWidget *parent = 0);
    ~route();
    int count = 0;
    int max_count = 0;
    void calculate();
    QVector<dataInfo> *data;
private:
    static constexpr int _ImgWidth=100;
    static constexpr int _ImgHeight=100;
    Ui::route *ui;
    QVBoxLayout *mainLayout;
    QHBoxLayout *bottomLayout;
    QPushButton *show;
    QImage _Img;
    QPoint start;
    QPoint end;
    bool ifdraw;
    int _countTable[_ImgWidth][_ImgHeight];
    int weight[_ImgWidth][_ImgHeight];
    bool block[_ImgWidth][_ImgHeight];
    void dijkstra(int start,int end);
    void print_path(int start,int end,int prev[]);

public slots:
    void on_show_clicked();
};

#endif // ROUTE_H
