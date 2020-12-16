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
    setFixedSize(540,560);

    mainLayout = new QVBoxLayout(this);
    bottomLayout = new QHBoxLayout(this);
    show = new QPushButton(this);
    show->setText("show");
    mainLayout->addSpacerItem(new QSpacerItem(10,540,QSizePolicy::Fixed,QSizePolicy::Expanding));
    mainLayout->addLayout(bottomLayout);
    bottomLayout->addSpacerItem(new QSpacerItem(440,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    bottomLayout->addWidget(show);

    _Img = QImage(QString("C:\\Users\\12779\\Desktop\\Final\\map-resized.PNG"));

    memset(_countTable,0,sizeof(int)*_ImgHeight*_ImgWidth);
    memset(weight,0,sizeof(int)*_ImgHeight*_ImgWidth);
    memset(block,0,sizeof(bool)*_ImgHeight*_ImgWidth);
    ifdraw = false;

    connect(show,SIGNAL(clicked(bool)),this,SLOT(on_show_clicked()));
}

route::~route()
{
    delete ui;
}

void route::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawImage(20,20,_Img);
    p.setPen(Qt::blue);
    QBrush brush(Qt::blue);
    p.setBrush(brush);
    if(count>=1){
        p.drawEllipse(start,3,3);
    }
    if(count>=2){
        p.drawEllipse(end,3,3);
    }
    p.setPen(Qt::transparent);
    p.setBrush(QColor(0, 0, 200, 120));
    if(ifdraw){
        for(int i = 0;i<_ImgWidth;++i){
            for(int j = 0;j<_ImgHeight;++j){
                if(block[i][j])
                    p.drawRoundRect(20+i*5,20+j*5,5,5,20,20);
            }
        }
    }
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
        qDebug()<<start;
        return;
    }

    print_path(start,prev[end],prev);
    block[end%_ImgWidth][end/_ImgWidth]=true;
    qDebug()<<'-'<<end;
 }
