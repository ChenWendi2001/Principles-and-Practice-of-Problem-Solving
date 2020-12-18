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
#include <QNetworkAccessManager>
#include <QNetWorkReply>
#include <QJsonDocument>
#include <QString>
#include <QJsonObject>
#include <QEventLoop>
#include <QJsonArray>
#include <QLabel>
#include <networkthread.h>
#include <QThread>
#include <QMutex>



namespace Ui {
class route;
}

class route : public QDialog
{
    mutable QMutex m_mutex;
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
    QVector<std::pair<double,double>> R;
    QVector<QString> road;
    QString duration;
private:
    static constexpr int _ImgWidth=100;
    static constexpr int _ImgHeight=100;
    Ui::route *ui;
    QVBoxLayout *mainLayout;
    QHBoxLayout *bottomLayout;
    QPushButton *show;
    QPushButton *AMap;
    QImage _Img;
    QPoint start;
    QPoint end;
    bool ifdraw;
    bool ifdrawroute;
    int _countTable[_ImgWidth][_ImgHeight];
    int weight[_ImgWidth][_ImgHeight];
    bool block[_ImgWidth][_ImgHeight];
    void dijkstra(int start,int end);
    void print_path(int start,int end,int prev[]);
    void Sleep(int msec);

public slots:
    void on_show_clicked();
    void a_map();
};

#endif // ROUTE_H
