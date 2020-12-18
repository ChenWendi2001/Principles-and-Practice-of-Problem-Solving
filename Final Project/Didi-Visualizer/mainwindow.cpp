#include "mainwindow.h"
#include "ui_mainwindow.h"

int STEP[4]={600,1800,3600,7200};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Didi Visualizer by Onlooker");
    setFixedSize(this->width(),this->height());
    ui->cancle_button->setEnabled(false);
    ui->order_id_check->setEnabled(false);
    ui->dep_info_check->setEnabled(false);


    connect(ui->load_button,SIGNAL(clicked(bool)),this,SLOT(on_load_clicked()));
    connect(ui->plot_button,SIGNAL(clicked(bool)),this,SLOT(on_plot_clicked()));
    //connect(ui->heat_button,SIGNAL(clicked(bool)),this,SLOT(on_heat_button_clicked()));

    chart = new QChart;
    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRubberBand(QChartView::HorizontalRubberBand);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);

    rect[0]=0;
    rect[1]=500;
    rect[2]=0;
    rect[3]=500;

    update_map();
    prepare();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_load_clicked()
{
    ui->load_button->setEnabled(false);
    ui->cancle_button->setEnabled(true);
    auto directory = QFileDialog::getExistingDirectory(
                      this, "choose a folder",
                       "/");
    auto FileThread = new fileThread;
    ui->progressBar->setRange(0,2943000);
    connect(ui->cancle_button,SIGNAL(clicked(bool)),FileThread,SLOT(changeFlag()));
    connect(FileThread,SIGNAL(changed(int)),ui->progressBar,SLOT(setValue(int)));
    connect(FileThread,SIGNAL(finished()),this,SLOT(reset()));
    FileThread->data = &data;
    FileThread->grid = &grid;
    FileThread->dir = directory;
    FileThread->start();

}

void MainWindow::reset()
{
    if(data.size() == 2943723){
        if(ui->dep_info_check->isChecked()){
            ui->graph_box->addItem("Real-time Demand");
        }
        if(ui->dest_info_check->isChecked()){
            ui->graph_box->addItem("Inflow-Outflow");
            ui->graph_box->addItem("Inflow-Outflow Ratio");
            ui->graph_box->addItem("Travel Time Distribution");
        }
        if(ui->fee_check->isChecked()){
            ui->graph_box->addItem("Fee Distribution");
            if(ui->dest_info_check->isChecked())ui->graph_box->addItem("Total Revenue");
        }

        ui->order_id_check->setEnabled(false);
        ui->dest_info_check->setEnabled(false);
        ui->fee_check->setEnabled(false);

        ifLoad = true;
        ui->load_button->setEnabled(false);
        ui->cancle_button->setEnabled(false);
        std::sort(data.begin(),data.end());
        ui->progressBar->setValue(2943000);

        qDebug()<<"sorting finished";
        QMessageBox::information(NULL, "Congrats!", "Data have been loaded successfully.",
                             QMessageBox::Ok);
        return;
    }
    ui->progressBar->setValue(0);
    ui->progressBar->update();
    ui->load_button->setEnabled(true);
    ui->cancle_button->setEnabled(false);
    data.clear();
    ifCancle = !ifCancle;
}

void MainWindow::on_plot_clicked()
{
    if(!ifLoad){
        QMessageBox::information(NULL, "Error", "Please load the data first.",
                             QMessageBox::Ok);
        return;
    }
    if(ui->graph_box->currentText()==QString("Real-time Demand"))print_num_per_time();
    if(ui->graph_box->currentText()==QString("Inflow-Outflow"))print_in_out();
    if(ui->graph_box->currentText()==QString("Inflow-Outflow Ratio"))print_in_out_ratio();
    if(ui->graph_box->currentText()==QString("Fee Distribution"))print_fee_distribution();
    if(ui->graph_box->currentText()==QString("Travel Time Distribution"))print_time_distribution();
    if(ui->graph_box->currentText()==QString("Total Revenue"))print_total_revenue();

}

void MainWindow::on_choosegrid_clicked()
{
    grid_dialog g;
    g.data = rect;
    g.exec();
    update_map();
}

void MainWindow::on_heat_button_clicked()
{

    QVector<dataInfo> selected;
    int gap = ui->start_time->dateTime().secsTo(ui->end_time->dateTime());
    if(!ifLoad){
        QMessageBox::information(NULL, "Error", "Please load the data first.",
                             QMessageBox::Ok);
        return;
    }
    if(gap<0){
        QMessageBox::information(NULL, "Error", "The time span should be positive.",
                             QMessageBox::Ok);
        return;
    }
    if(gap>5*3600*24){
        QMessageBox::information(NULL, "Error", "The time span is too long.",
                             QMessageBox::Ok);
        return;
    }
    if(gap+3599<4*STEP[ui->step_box->currentIndex()]){
        QMessageBox::information(NULL, "Error", "The time span is too short.",
                             QMessageBox::Ok);
        return;
    }
    else{

        int current = ui->start_time->dateTime().toTime_t();
        int end=ui->end_time->dateTime().toTime_t()+3599;
        for(dataInfo d:data){
            if(d.dep_time>=current && d.dep_time<=end && map[d.i][d.j])
                selected.append(d);
        }
    }
    qDebug()<<selected.size();
    heat h;
    h.step = STEP[ui->step_box->currentIndex()];
    h.gap = gap+3599;
    h.d = &selected;
    h.calculate();
    h.exec();
}

void MainWindow::on_route_button_clicked()
{
    if(!ifLoad){
        QMessageBox::information(NULL, "Error", "Please load the data first.",
                             QMessageBox::Ok);
        return;
    }
    QVector<dataInfo> selected;
    int current = ui->start_time->dateTime().toTime_t();
    int end=ui->end_time->dateTime().toTime_t()+3599;
    for(dataInfo d:data){
        if(d.dep_time>=current && d.dep_time<=end)
            selected.append(d);
    }
    route *r = new route;
    r->data = &selected;
    r->calculate();
    r->open();
    connect(r, &QDialog::finished, [r, this](int result) {
        r->deleteLater();
    });
}

void MainWindow::update_map()
{

    for(int i = 0;i<10;++i){
        for(int j = 0;j<10;++j){
            if(i*50>=(rect[0]-rect[0]%50)&&i*50<=(rect[1]-rect[1]%50+50)&&
                    j*50>=(rect[2]-rect[2]%50)&&j*50<=(rect[3]-rect[3]%50+50))
                map[i][j]=true;
            else
                map[i][j]=false;
        }
    }
}

void MainWindow::print_num_per_time()
{
    int gap = ui->start_time->dateTime().secsTo(ui->end_time->dateTime());
    //qDebug()<<gap;
    if(!ifLoad){
        QMessageBox::information(NULL, "Error", "Please load the data first.",
                             QMessageBox::Ok);
        return;
    }
    if(gap<0){
        QMessageBox::information(NULL, "Error", "The time span should be positive.",
                             QMessageBox::Ok);
        return;
    }
    if(gap+3599<4*STEP[ui->step_box->currentIndex()]){
        QMessageBox::information(NULL, "Error", "The time span is too short.",
                             QMessageBox::Ok);
        return;
    }
    else{
        QVector<double> num_per_inter;
        QVector<double> time;
        int current = ui->start_time->dateTime().toTime_t();
        int end=ui->end_time->dateTime().toTime_t();
        int count = 0;


        for(dataInfo d:data){

            if(d.i<0||d.i>9||d.j<0||d.j>9||!map[d.i][d.j])
                continue;

            if(d.dep_time<current){
                continue;
            }
            if(d.dep_time>end+3599){
                time.append(current);
                num_per_inter.append(count);
                break;
            }
            if(d.dep_time >= current+STEP[ui->step_box->currentIndex()]){
                time.append(current);
                num_per_inter.append(count);
                count = 1;
                current+=STEP[ui->step_box->currentIndex()];
                continue;
            }
            count++;
        }


        //qDebug()<<ui->start_time->dateTime().toTime_t();
        //qDebug()<<ui->start_time->dateTime().toMSecsSinceEpoch();
        //qDebug()<<num_per_inter;
        //qDebug()<<time;
        auto series = new QSplineSeries(this);
        series->setName("Real-time Demand");
        Lagrange l(time,num_per_inter);

        for(int i = 0;i<time.size();++i){
            series->append(time[i]*1000,num_per_inter[i]);

            if(time.size()<50&&abs(i-time.size()/2)<time.size()/4){
                int n = 50/time.size();
                for(int j = 2;j<n-1;j++){
                    series->append((time[i]+STEP[ui->step_box->currentIndex()]*j/n)*1000,l.get_interp_x(time[i]+STEP[ui->step_box->currentIndex()]*j/n));
                }
            }

        }

        chart->removeAllSeries();
        if(axisX != nullptr)chart->removeAxis(axisX);
        axisX = nullptr;
        if(axisY != nullptr)chart->removeAxis(axisY);
        axisY = nullptr;
        if(axisX_another != nullptr)chart->removeAxis(axisX_another);
        axisX_another = nullptr;
        chart->addSeries(series);
        axisX = new QDateTimeAxis(this);
        axisY = new QValueAxis(this);
        axisX->setFormat("MM-dd h");
        axisY->setTitleText("Demand Per Time Step");
        axisX->setTitleText("Time");
        chart->setAxisX(axisX,series);
        chart->setAxisY(axisY,series);
        chart->setAnimationOptions(QChart::SeriesAnimations);
    }
}

void MainWindow::print_in_out()
{
    int gap = ui->start_time->dateTime().secsTo(ui->end_time->dateTime());
    //qDebug()<<gap;
    if(!ifLoad){
        QMessageBox::information(NULL, "Error", "Please load the data first.",
                             QMessageBox::Ok);
        return;
    }
    if(gap<0){
        QMessageBox::information(NULL, "Error", "The time span should be positive.",
                             QMessageBox::Ok);
        return;
    }
    if(gap+3599<4*STEP[ui->step_box->currentIndex()]){
        QMessageBox::information(NULL, "Error", "The time span is too short.",
                             QMessageBox::Ok);
        return;
    }
    else{

        int num = (gap+3599)/STEP[ui->step_box->currentIndex()]+1;
        QVector<int> outflow;
        for(int i = 0;i<num;++i){
            outflow.push_back(0);
        }

        QVector<int> inflow;
        for(int i = 0;i<num;++i){
            inflow.push_back(0);
        }

        QVector<double> time;
        int current = ui->start_time->dateTime().toTime_t();
        for(int i = 0;i<num;++i){
            time.push_back(current);
            current+=STEP[ui->step_box->currentIndex()];
        }

        current = ui->start_time->dateTime().toTime_t();
        int end = ui->end_time->dateTime().toTime_t()+3599;



        for(dataInfo d:data){
            if(d.dep_time>=current && d.dep_time<=end){
                if(d.i>=0&&d.i<=10&&d.j>=0&&d.j<=10&&map[d.i][d.j]){
                    int k = double(abs(d.dep_time-current-1))/(end-current)*num;
                    outflow[k]++;
                }
            }

            if(d.end_time>=current && d.end_time<=end){
                if(d.l_i>=0&&d.l_i<=10&&d.l_j>=0&&d.l_j<=10&&map[d.l_i][d.l_j]){
                    int k = double(abs(d.end_time-current-1))/(end-current)*num;
                    inflow[k]++;
                }
            }
        }

        //qDebug()<<inflow;
        //qDebug()<<outflow;

        auto series1 = new QLineSeries(this);

        auto series2 = new QLineSeries(this);

        auto series3 = new QLineSeries(this);



        for(int i = 0;i<time.size();++i){
            series1->append(QPointF(time[i]*1000,inflow[i]+outflow[i]));
            series2->append(QPointF(time[i]*1000,inflow[i]));
            series3->append(QPointF(time[i]*1000,0));
        }

        auto series = new QAreaSeries(series2,series1);
        series->setName("outflow");

        auto _series = new QAreaSeries(series3,series2);
        _series->setName("inflow");

        chart->removeAllSeries();
        if(axisX != nullptr)chart->removeAxis(axisX);
        axisX = nullptr;
        if(axisY != nullptr)chart->removeAxis(axisY);
        axisY = nullptr;
        if(axisX_another != nullptr)chart->removeAxis(axisX_another);
        axisX_another = nullptr;

        chart->addSeries(series);
        chart->addSeries(_series);

        axisX = new QDateTimeAxis(this);
        axisX->setFormat("MM-dd h");
        axisY = new QValueAxis(this);
        axisY->setTitleText("Flow");
        axisX->setTitleText("Time");
        axisX->setLabelsAngle(-45);
        chart->setAxisX(axisX,series);
        chart->setAxisY(axisY);

        series->attachAxis(axisY);
        _series->attachAxis(axisY);

        chart->setAnimationOptions(QChart::SeriesAnimations);
    }
}


void MainWindow::print_in_out_ratio()
{
    int gap = ui->start_time->dateTime().secsTo(ui->end_time->dateTime());
    //qDebug()<<gap;
    if(!ifLoad){
        QMessageBox::information(NULL, "Error", "Please load the data first.",
                             QMessageBox::Ok);
        return;
    }
    if(gap<0){
        QMessageBox::information(NULL, "Error", "The time span should be positive.",
                             QMessageBox::Ok);
        return;
    }
    if(gap+3599<4*STEP[ui->step_box->currentIndex()]){
        QMessageBox::information(NULL, "Error", "The time span is too short.",
                             QMessageBox::Ok);
        return;
    }
    else{

        int num = (gap+3599)/STEP[ui->step_box->currentIndex()]+1;
        QVector<int> outflow;
        for(int i = 0;i<num;++i){
            outflow.push_back(0);
        }

        QVector<int> inflow;
        for(int i = 0;i<num;++i){
            inflow.push_back(0);
        }

        QVector<double> time;
        int current = ui->start_time->dateTime().toTime_t();
        for(int i = 0;i<num;++i){
            time.push_back(current);
            current+=STEP[ui->step_box->currentIndex()];
        }

        current = ui->start_time->dateTime().toTime_t();
        int end = ui->end_time->dateTime().toTime_t()+3599;



        for(dataInfo d:data){
            if(d.dep_time>=current && d.dep_time<=end){
                if(d.i>=0&&d.i<=10&&d.j>=0&&d.j<=10&&map[d.i][d.j]){
                    int k = double(abs(d.dep_time-current-1))/(end-current)*num;
                    outflow[k]++;
                }
            }

            if(d.end_time>=current && d.end_time<=end){
                if(d.l_i>=0&&d.l_i<=10&&d.l_j>=0&&d.l_j<=10&&map[d.l_i][d.l_j]){
                    int k = double(abs(d.end_time-current-1))/(end-current)*num;
                    inflow[k]++;
                }
            }
        }

        //qDebug()<<inflow;
        //qDebug()<<outflow;

        auto series = new QStackedBarSeries(this);
        series->setName("Inflow-Outflow Ratio");

        QBarSet *set0 = new QBarSet("Inflow");
        QBarSet *set1 = new QBarSet("Outflow");

        for(int i = 0;i<time.size();++i){
            set0->append(double(inflow[i])/(inflow[i]+outflow[i]));
            set1->append(double(outflow[i])/(inflow[i]+outflow[i]));
        }

        series->append(set0);
        series->append(set1);

        chart->removeAllSeries();
        if(axisX != nullptr)chart->removeAxis(axisX);
        axisX = nullptr;
        if(axisY != nullptr)chart->removeAxis(axisY);
        axisY = nullptr;
        if(axisX_another != nullptr)chart->removeAxis(axisX_another);
        axisX_another = nullptr;
        chart->addSeries(series);

        QStringList categories;
        for(int t:time){
            categories.append(QDateTime::fromTime_t(t).toString("dd h:mm"));
        }
        //qDebug()<<categories;
        axisX_another = new QBarCategoryAxis();
        axisX_another->append(categories);

        axisY = new QValueAxis(this);
        axisY->setTitleText("Ratio");
        axisX_another->setTitleText("Time");
        axisX_another->setLabelsAngle(-45);


        chart->setAxisX(axisX_another);
        chart->setAxisY(axisY,series);

        series->attachAxis(axisX_another);

        chart->setAnimationOptions(QChart::SeriesAnimations);
    }
}

void MainWindow::print_time_distribution(){
    int gap = ui->start_time->dateTime().secsTo(ui->end_time->dateTime());
    //qDebug()<<gap;
    if(!ifLoad){
        QMessageBox::information(NULL, "Error", "Please load the data first.",
                             QMessageBox::Ok);
        return;
    }
    if(gap<0){
        QMessageBox::information(NULL, "Error", "The time span should be positive.",
                             QMessageBox::Ok);
        return;
    }
    else{
        QVector<int> dist;
        int current = ui->start_time->dateTime().toTime_t();
        int end=ui->end_time->dateTime().toTime_t();


        for(int i = 0;i<4;++i) dist.push_back(0);


        for(dataInfo d:data){

            if(d.dep_time>=current && d.dep_time<=end+3599 && d.i>=0&&d.i<=10&&d.j>=0&&d.j<=10&&map[d.i][d.j]){
                if(d.end_time - d.dep_time<600){
                    dist[0]++;
                }
                else if(d.end_time - d.dep_time<1800){
                    dist[1]++;
                }
                else if(d.end_time - d.dep_time<3600){
                    dist[2]++;
                }
                else{
                    dist[3]++;
                }
            }
        }


        auto series = new QPieSeries(this);

        series->setName("Real-time Demand");
        series->append("0-10 min",dist[0]);
        series->append("10-30 min",dist[1]);
        series->append("30-60 min",dist[2]);
        series->append("over 60 min",dist[3]);
        series->setLabelsVisible(true);
        chart->removeAllSeries();
        if(axisX != nullptr)chart->removeAxis(axisX);
        axisX = nullptr;
        if(axisY != nullptr)chart->removeAxis(axisY);
        axisY = nullptr;
        if(axisX_another != nullptr)chart->removeAxis(axisX_another);
        axisX_another = nullptr;
        chart->addSeries(series);

        chart->setAnimationOptions(QChart::SeriesAnimations);
    }
}

void MainWindow::print_fee_distribution()
{
    int gap = ui->start_time->dateTime().secsTo(ui->end_time->dateTime());
    //qDebug()<<gap;
    if(!ifLoad){
        QMessageBox::information(NULL, "Error", "Please load the data first.",
                             QMessageBox::Ok);
        return;
    }
    if(gap<0){
        QMessageBox::information(NULL, "Error", "The time span should be positive.",
                             QMessageBox::Ok);
        return;
    }
    else{
        QVector<int> dist;
        int current = ui->start_time->dateTime().toTime_t();
        int end=ui->end_time->dateTime().toTime_t();


        for(int i = 0;i<3;++i) dist.push_back(0);


        for(dataInfo d:data){

            if(d.dep_time>=current && d.dep_time<=end+3599 && d.i>=0&&d.i<=10&&d.j>=0&&d.j<=10&&map[d.i][d.j]){
                if(d.fee<3){
                    dist[0]++;
                }
                else if(d.fee<10){
                    dist[1]++;
                }
                else{
                    dist[2]++;
                }
            }
        }


        auto series = new QPieSeries(this);

        series->setName("Real-time Demand");
        series->append("0-3",dist[0]);
        series->append("3-10",dist[1]);
        series->append("over 10",dist[2]);
        series->setLabelsVisible(true);
        chart->removeAllSeries();
        if(axisX != nullptr)chart->removeAxis(axisX);
        axisX = nullptr;
        if(axisY != nullptr)chart->removeAxis(axisY);
        axisY = nullptr;
        if(axisX_another != nullptr)chart->removeAxis(axisX_another);
        axisX_another = nullptr;

        chart->addSeries(series);
        chart->setAnimationOptions(QChart::SeriesAnimations);
    }
}

void MainWindow::print_total_revenue()
{
    int gap = ui->start_time->dateTime().secsTo(ui->end_time->dateTime());
    //qDebug()<<gap;
    if(!ifLoad){
        QMessageBox::information(NULL, "Error", "Please load the data first.",
                             QMessageBox::Ok);
        return;
    }
    if(gap<0){
        QMessageBox::information(NULL, "Error", "The time span should be positive.",
                             QMessageBox::Ok);
        return;
    }
    if(gap+3599<4*STEP[ui->step_box->currentIndex()]){
        QMessageBox::information(NULL, "Error", "The time span is too short.",
                             QMessageBox::Ok);
        return;
    }
    else{

        int num = (gap+3599)/STEP[ui->step_box->currentIndex()]+1;
        QVector<int> revenue;
        for(int i = 0;i<num;++i){
            revenue.push_back(0);
        }

        QVector<double> time;
        int current = ui->start_time->dateTime().toTime_t();
        for(int i = 0;i<num;++i){
            time.push_back(current);
            current+=STEP[ui->step_box->currentIndex()];
        }

        current = ui->start_time->dateTime().toTime_t();
        int end = ui->end_time->dateTime().toTime_t()+3599;


        for(dataInfo d:data){
            if(d.dep_time>=current && d.end_time<=end){
                if(d.i>=0&&d.i<=10&&d.j>=0&&d.j<=10&&map[d.i][d.j]){
                    int k = double(abs(d.dep_time-current-1))/(end-current)*num;
                    revenue[k]+=d.fee;
                }
            }
        }


        auto series = new QBarSeries(this);
        series->setName("Total Revenue");

        QBarSet *set0 = new QBarSet("revenue");

        for(int i = 0;i<time.size();++i){
            set0->append(revenue[i]);
        }

        series->append(set0);

        chart->removeAllSeries();
        if(axisX != nullptr)chart->removeAxis(axisX);
        axisX = nullptr;
        if(axisY != nullptr)chart->removeAxis(axisY);
        axisY = nullptr;
        if(axisX_another != nullptr)chart->removeAxis(axisX_another);
        axisX_another = nullptr;
        chart->addSeries(series);

        QStringList categories;
        for(int t:time){
            categories.append(QDateTime::fromTime_t(t).toString("dd h:mm"));
        }
        //qDebug()<<categories;
        axisX_another = new QBarCategoryAxis();
        axisX_another->append(categories);

        axisY = new QValueAxis(this);
        axisY->setTitleText("Ratio");
        axisX_another->setTitleText("Time");
        axisX_another->setLabelsAngle(-45);

        chart->setAxisX(axisX_another);
        chart->setAxisY(axisY,series);
        series->attachAxis(axisX_another);
        chart->setAnimationOptions(QChart::SeriesAnimations);
    }
}

void MainWindow::prepare()
{
    QString url = QString("https://restapi.amap.com/v3/direction/driving?origin=104.129835,30.697831&destination=103.972389,30.674089&extensions=all&output=json&key=25824a502a0e2666ffe1b3744dbe8806");
    qDebug()<<url;
    auto netManager = new QNetworkAccessManager(this);
    QEventLoop loop;
    QObject::connect(netManager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkRequest request;
    request.setUrl(url);
    QCoreApplication::processEvents();
    auto reply = netManager->get(request);
    loop.exec();
}
