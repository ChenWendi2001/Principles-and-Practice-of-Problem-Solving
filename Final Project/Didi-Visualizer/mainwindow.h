#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <datainfo.h>
#include <grid.h>
#include <filethread.h>
#include <QtCharts>
#include <lagrange.h>
#include <grid_dialog.h>
#include <heat.h>
#include <route.h>

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
    QChart* chart;

    QDateTimeAxis *axisX=nullptr;
    QValueAxis *axisY=nullptr;
    QBarCategoryAxis *axisX_another = nullptr;

    QVector<dataInfo> data;
    QVector<Grid> grid;
    int rect[4];
    bool map[10][10];
    bool ifCancle = false;
    bool ifLoad = false;

    void update_map();
    void print_num_per_time();
    void print_in_out();
    void print_in_out_ratio();
    void print_time_distribution();
    void print_fee_distribution();
    void print_total_revenue();
public slots:
    void on_load_clicked();
    void on_plot_clicked();
    void on_choosegrid_clicked();
    void on_heat_button_clicked();
    void on_route_button_clicked();
    void reset();
   // void change_if_cancle();


};

#endif // MAINWINDOW_H
