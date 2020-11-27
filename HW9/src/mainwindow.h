#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSizePolicy>
#include <QString>
#include <QStatusBar>
#include <QFile>
#include <vector>
#include <QTextStream>
#include <QStringList>
#include <QtCharts>
#include <QMenu>
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget * widget;
    QBoxLayout *outerLayout;
    QHBoxLayout *topLayout;
    QHBoxLayout *secondLayout;
    QHBoxLayout *thirdLayout;
    QComboBox *directoryComboBox;
    QComboBox *startHour;
    QComboBox *endHour;
    QPushButton *select ;
    QPushButton *start;
    QChart* chart;
    QChartView * chartview;
    QLineSeries * series;
    QValueAxis *axisX;
    QValueAxis *axisY;

    void Init_UI();
    void Init_Signal_Slots();

    std::vector<std::pair<int,int>> data;


public slots:
    void Select_File();
    void Assert_Validity();
    void Load_Data();
    void Update_Data();

};

#endif // MAINWINDOW_H
