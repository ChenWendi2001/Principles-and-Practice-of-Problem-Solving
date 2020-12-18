#include "heat.h"
#include "ui_heat.h"

heat::heat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::heat)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setWindowTitle("Here's the thermal diagram");
    setFixedSize(ImgWidth+40,ImgHeight+100);

    _Img = QImage(QString("C:\\Users\\12779\\Desktop\\Final\\map-resized.PNG"));

    time_slider = new QSlider(this);
    time_slider->setOrientation(Qt::Orientation::Horizontal);
    time_slider->setRange(0,99);
    main_layout = new QVBoxLayout(this);
    main_layout->addSpacerItem(new QSpacerItem(10,540,QSizePolicy::Expanding,QSizePolicy::Fixed));
    main_layout->addWidget(time_slider);
    setLayout(main_layout);

    show = new QPushButton("show");
    time = new QLabel("time");
    bottom_layout = new QHBoxLayout(this);
    bottom_layout->addWidget(time);
    bottom_layout->addSpacerItem(new QSpacerItem(440,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    bottom_layout->addWidget(show);
    main_layout->addLayout(bottom_layout);

    _dataImg=QImage(ImgWidth,ImgHeight,QImage::Format_Alpha8);
    _dataImg.fill(Qt::transparent);

    _heatImg=QImage(ImgWidth,ImgHeight,QImage::Format_ARGB32);
    _heatImg.fill(Qt::transparent);

    QLinearGradient linear=QLinearGradient(QPoint(0,0),QPoint(255,0));
    linear.setColorAt(0, Qt::blue);
    linear.setColorAt(0.4, Qt::blue);
    linear.setColorAt(0.5, Qt::cyan);
    linear.setColorAt(0.6, Qt::green);
    linear.setColorAt(0.8, Qt::yellow);
    linear.setColorAt(0.95, Qt::red);

    QImage img(256,1,QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.fillRect(img.rect(),linear);

    quint32 alpha=0;
    for(quint32 i=0;i<256;i++){
        alpha=HeatAlpha/255.0*i;
        _colorList[i]=img.pixel(i,0)&0x00FFFFFF|(alpha<<24);
    }

    connect(time_slider,SIGNAL(valueChanged(int)),this,SLOT(update_heat_map()));
    connect(show,SIGNAL(clicked(bool)),this,SLOT(show_heat_map()));

}

heat::~heat()
{
    delete ui;
}

void heat::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawImage(20,20,_Img);
    p.drawImage(20,20,_heatImg);
    QDialog::paintEvent(event);
}

void heat::calculate()
{
    _maxCount=1;
    memset(_countTable,0,ImgWidth*ImgHeight*sizeof(int));

    if(d == nullptr) return;

    int start = d->at(0).dep_time;
    int end = d->at(0).dep_time+gap;

    start +=int(gap*time_slider->value()/99);

    if(start+step>end){
        start = end-step;
    }
    else
    {
        end = start+step;
    }

    time->setText(QString("Time:")+QDateTime::fromTime_t(start).toString("MM-dd hh:mm"));

    for(int i = 0;i<d->size()-1;++i){
        if(d->at(i).dep_time<start||d->at(i).dep_time>end) continue;
        const Point pt{d->at(i).index_i,d->at(i).index_j,_radius,1};
        const int pos_count=appendPoint(pt);

        if(pos_count>1){
            if(pos_count>_maxCount)
                _maxCount=pos_count;
        }
    }
    drawDataImg();
    drawHeatImg();
    update();
}

int heat::appendPoint(const Point &pt)
{
    if(pt.posX<0||pt.posY<0||pt.radius<0||
            pt.posX>=ImgWidth||pt.posY>=ImgHeight)
        return 0;

    _countTable[pt.posX+pt.posY*ImgWidth]++;
    return _countTable[pt.posX+pt.posY*ImgWidth];
}

void heat::drawDataImg()
{
    _dataImg.fill(Qt::transparent);
    QPainter painter(&_dataImg);
    painter.setPen(Qt::transparent);
    const double max_count=_maxCount;
    for(int i=0;i<ImgHeight*ImgWidth;i++)
    {
        if(!_countTable[i]) continue;
        const uchar alpha=uchar(_countTable[i]/max_count*255);
        QRadialGradient gradient(i%ImgWidth,i/ImgWidth,_radius);
        gradient.setColorAt(0,QColor(0,0,0,alpha));
        gradient.setColorAt(1,QColor(0,0,0,0));
        painter.setBrush(gradient);
        painter.drawEllipse(QPointF(i%ImgWidth,i/ImgWidth),_radius,_radius);
    }
}

void heat::drawHeatImg()
{
    for(int row=0;row<_dataImg.height();row++)
    {
        const uchar *line_data=_dataImg.scanLine(row);
        QRgb *line_heat=reinterpret_cast<QRgb*>(_heatImg.scanLine(row));
        for(int col=0;col<_dataImg.width();col++)
        {
            line_heat[col]=_colorList[line_data[col]];
        }
    }
}

void heat::update_heat_map()
{
    calculate();
    update();
}

void heat::show_heat_map(){
    time_slider->setEnabled(false);
    show->setEnabled(false);
    for(int i = 4;i<=99;i+=5){
        Sleep(250);
        time_slider->setValue(i);
    }
    time_slider->setEnabled(true);
    show->setEnabled(true);
}

void heat::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

