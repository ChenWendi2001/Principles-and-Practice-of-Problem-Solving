#include "route.h"
#include "ui_route.h"

route::route(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::route)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setWindowTitle("Please Click to Show the Route");
    setFixedSize(800,560);

    mainLayout = new QVBoxLayout(this);
    bottomLayout = new QHBoxLayout(this);
    show = new QPushButton(this);
    AMap = new QPushButton(this);
    show->setText("Dijkstra");
    AMap->setText("AMap");
    mainLayout->addSpacerItem(new QSpacerItem(10,540,QSizePolicy::Fixed,QSizePolicy::Expanding));
    mainLayout->addLayout(bottomLayout);
    bottomLayout->addSpacerItem(new QSpacerItem(440,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    bottomLayout->addWidget(AMap);
    bottomLayout->addWidget(show);

    _Img = QImage(QString("C:\\Users\\12779\\Desktop\\Final\\map-resized.PNG"));

    memset(_countTable,0,sizeof(int)*_ImgHeight*_ImgWidth);
    memset(weight,0,sizeof(int)*_ImgHeight*_ImgWidth);
    memset(block,0,sizeof(bool)*_ImgHeight*_ImgWidth);
    ifdraw = false;
    ifdrawroute = false;

    connect(show,SIGNAL(clicked(bool)),this,SLOT(on_show_clicked()));
    connect(AMap,SIGNAL(clicked(bool)),this,SLOT(a_map()));
}

route::~route()
{
    delete ui;
}

void route::paintEvent(QPaintEvent *event)
{
    //m_mutex.lock();
    //const QMutexLocker locker(&m_mutex);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.drawImage(20,20,_Img);
    QBrush brush(Qt::blue);
    p.setBrush(brush);
    if(count>=1){
        p.drawEllipse(start,3,3);
    }
    if(count>=2){
        p.drawEllipse(end,3,3);
    }
    p.setPen(Qt::transparent);
    p.setBrush(QColor(0, 0, 200));
    if(ifdraw){
        for(int i = 0;i<_ImgWidth;++i){
            for(int j = 0;j<_ImgHeight;++j){
                if(block[i][j])
                    p.drawRoundRect(20+i*5,20+j*5,5,5,20,20);
            }
        }
    }
    if(ifdrawroute){
        ui->label->setText(QString("Time Cost:")+duration+QString("s"));
        p.setPen(QPen(Qt::blue,5));
        for(int i = 0;i<R.size()-1;++i){
            //qDebug()<<R.at(i);
//            int start_x = (R.at(i).first - 103.908407474531)/(104.222044525468-103.908407474531)*500+20;
//            int end_x = (R.at(i+1).first - 103.908407474531)/(104.222044525468-103.908407474531)*500+20;
//            int start_y = 520-(R.at(i).second - 30.524081949676)/(30.7938780503239-30.524081949676)*500;
//            int end_y = 520-(R.at(i+1).second- 30.524081949676)/(30.7938780503239-30.524081949676)*500;;
            int start_x = (R.at(i).first - s_x)/(e_x-s_x)*500+20;
            int end_x = (R.at(i+1).first - s_x)/(e_x-s_x)*500+20;
            int start_y = 520-(R.at(i).second - s_y)/(e_y-s_y)*500;
            int end_y = 520-(R.at(i+1).second- s_y)/(e_y-s_y)*500;;
            p.drawLine(start_x,start_y,end_x,end_y);
        }
//        for(int i = 0;i<road.size()-1;++i){
//            ui->routelist->addItem(road.at(i));
//            //qDebug()<<road.at(i);
//        }
    }
    //m_mutex.unlock();
}


void route::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    QPoint pos=event->pos();
    qDebug()<<pos;
    if(count==0){
        count++;
        start = pos;
        update();
    }
    else if(count==1){
        count++;
        end = pos;
        update();
    }
}

void route::calculate()
{

    for(int i = 0;i<data->size();++i){
        if(data->at(i).r_index_i<0||data->at(i).r_index_j<0||data->at(i).r_index_i>99||data->at(i).r_index_j>99) continue;
        _countTable[data->at(i).r_index_i][data->at(i).r_index_j]++;
        if(max_count<_countTable[data->at(i).r_index_i][data->at(i).r_index_j])
            max_count=_countTable[data->at(i).r_index_i][data->at(i).r_index_j];
    }

    qDebug()<<max_count;

    for(int i = 0;i<_ImgWidth;++i){
        for(int j = 0;j<_ImgHeight;++j){
            weight[i][j] = 1+double(_countTable[i][j])/max_count*20;
        }
    }

}

void route::on_show_clicked()
{
    if(count==2){
        show->setEnabled(false);
        AMap->setEnabled(false);
        int s = (start.x()-20)/5+_ImgWidth*((start.y()-20)/5);
        int e = (end.x()-20)/5+_ImgWidth*((end.y()-20)/5);
        dijkstra(s,e);
        ifdraw = true;
        update();
    }
}

void route::dijkstra(int start,int end)
{
    int vers = _ImgWidth*_ImgHeight;
    int noEdge = 10000000;
    int *distance = new int[vers];
    int *prev = new int[vers];
    bool *known = new bool[vers];

    int u,i,j;
    int min;

    for(int i = 0;i<vers;++i){
        known[i] = false;
        distance[i] = noEdge;
    }

    distance[start] = 0;
    prev[start] = start;

    for(i = 1;i<vers;++i){
        min = noEdge;
        for(j = 0;j<vers;++j){
            if(!known[j]&&distance[j]<min){
                min = distance[j];
                u = j;
            }
        }
        known[u] = true;
        if(u%_ImgWidth>0){
            if(!known[u-1]&&distance[u-1]>min+weight[u%_ImgWidth-1][u/_ImgWidth]){
                distance[u-1] = min+weight[u%_ImgWidth-1][u/_ImgWidth];
                prev[u-1] = u;
            }
        }
        if(u%_ImgWidth<_ImgWidth-1){
            if(!known[u+1]&&distance[u+1]>min+weight[u%_ImgWidth+1][u/_ImgWidth]){
                distance[u+1] = min+weight[u%_ImgWidth+1][u/_ImgWidth];
                prev[u+1] = u;
            }
        }
        if(u/_ImgWidth>0){
            if(!known[u-_ImgWidth]&&distance[u-_ImgWidth]>min+weight[u%_ImgWidth][u/_ImgWidth-1]){
                distance[u-_ImgWidth] = min+weight[u%_ImgWidth][u/_ImgWidth-1];
                prev[u-_ImgWidth] = u;
            }
        }
        if(u/_ImgWidth<_ImgHeight-1){
            if(!known[u+_ImgWidth]&&distance[u+_ImgWidth]>min+weight[u%_ImgWidth][u/_ImgWidth+1]){
                distance[u+_ImgWidth] = min+weight[u%_ImgWidth][u/_ImgWidth+1];
                prev[u+_ImgWidth] = u;
            }
        }
    }


    print_path(start,end,prev);

    delete []distance;
    delete []prev;
    delete []known;
}

 void route::print_path(int start, int end, int prev[])
 {
    if(start==end){
        block[start%_ImgWidth][start/_ImgWidth]=true;
        //qDebug()<<start;
        return;
    }

    print_path(start,prev[end],prev);
    block[end%_ImgWidth][end/_ImgWidth]=true;
    //qDebug()<<'-'<<end;
 }

 void route::a_map(){
    //const QMutexLocker locker(&m_mutex);

    if(count ==2){
        m_mutex.lock();
        show->setEnabled(false);
        AMap->setEnabled(false);
//        double start_x = (double(start.x())-20)/500.0*(104.222044525468-103.908407474531)+103.90840747453;
//        double start_y = (500-(double(start.y())-20))/500.0*(30.7938780503239-30.524081949676)+30.524081949676;
//        double end_x = (double(end.x())-20)/500.0*(104.222044525468-103.908407474531)+103.90840747453;
//        double end_y = (500-(double(end.y())-20))/500.0*(30.7938780503239-30.524081949676)+30.524081949676;
        double start_x = (double(start.x())-20)/500.0*(e_x-s_x)+s_x;
        double start_y = (500-(double(start.y())-20))/500.0*(e_y-s_y)+s_y;
        double end_x = (double(end.x())-20)/500.0*(e_x-s_x)+s_x;
        double end_y = (500-(double(end.y())-20))/500.0*(e_y-s_y)+s_y;

        QThread* m_workerThread = new QThread();
        networkThread* worker = new networkThread();
        worker->start_x = start_x;
        worker->start_y = start_y;
        worker->end_x = end_x;
        worker->end_y = end_y;
        worker->R = &R;
        worker->road = &road;
        worker->duration = &duration;
        worker->m_list = ui->routelist;
        worker->moveToThread(m_workerThread);

        connect(m_workerThread, &QThread::started, worker, &networkThread::start1);
        connect(worker, &networkThread::workFinished, worker, &networkThread::deleteLater);
        connect(worker, &networkThread::workFinished, m_workerThread, &QThread::quit);
        connect(m_workerThread, &QThread::finished, m_workerThread, &QThread::deleteLater);

        m_workerThread->start();
        Sleep(1000);

        ifdrawroute = true;
        m_mutex.unlock();
        update();
    }
 }
 void route::Sleep(int msec)
 {
     QTime dieTime = QTime::currentTime().addMSecs(msec);
     while( QTime::currentTime() < dieTime )
         QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
 }
