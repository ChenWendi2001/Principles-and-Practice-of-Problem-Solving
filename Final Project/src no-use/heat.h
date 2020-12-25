#ifndef HEAT_H
#define HEAT_H

#include <QDialog>
#include <QVector>
#include <QImage>
#include <QList>
#include <QRgb>
#include <QPainter>
#include <QPaintEvent>
#include <QSlider>
#include <QBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QLabel>
#include <datainfo.h>

namespace Ui {
class heat;
}

class heat : public QDialog
{
    Q_OBJECT
    struct Point
    {
        int posX;
        int posY;
        int radius;
        int count;
    };
public:
    explicit heat(QWidget *parent = 0);
    ~heat();
    QVector<dataInfo> *d = nullptr;
    int step = 0;
    int gap = 0;
    void calculate();

private:
    const int HeatAlpha=200;
    const int ImgWidth = 500;
    const int ImgHeight = 500;
    const int _radius = 10;



    Ui::heat *ui;
    QSlider *time_slider;
    QVBoxLayout *main_layout;
    QHBoxLayout *bottom_layout;
    QPushButton *show;
    QLabel *time;

    QList<Point> _posList;
    QImage _Img;
    QImage _dataImg;
    QImage _heatImg;
    QRgb _colorList[256];
    int _maxCount=1;
    int *_countTable=new int[ImgWidth*ImgHeight]();

    void paintEvent(QPaintEvent *event) override;
    int appendPoint(const Point &pt);
    void drawDataImg();
    void drawDataPoint(const Point &pt);
    void drawHeatImg();
    void Sleep(int msec);
public slots:

    void update_heat_map();
    void show_heat_map();


};

#endif // HEAT_H
