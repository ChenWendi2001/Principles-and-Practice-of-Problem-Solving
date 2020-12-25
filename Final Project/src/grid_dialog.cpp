#include "grid_dialog.h"
#include "ui_grid_dialog.h"

grid_dialog::grid_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::grid_dialog)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setWindowTitle("Please Choose Grids");
    setFixedSize(525,560);

    m_label = new MyLabel(this);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_label);

    bottomlayout = new QHBoxLayout(this);
    OK = new QPushButton("OK");
    Cancle = new QPushButton("Cancle");
    OK->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    Cancle->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    bottomlayout->addSpacerItem(new QSpacerItem(200,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    bottomlayout->addWidget(OK);
    bottomlayout->addWidget(Cancle);

    mainLayout->addLayout(bottomlayout);

    this->setLayout(mainLayout);

    QImage image(QString(".\\lib\\map-resized.PNG"));

    m_label->setBackImage(image);

    connect(OK,SIGNAL(clicked(bool)),this,SLOT(on_ok_clicked()));
    connect(Cancle,SIGNAL(clicked(bool)),this,SLOT(on_ok_clicked()));
}

grid_dialog::~grid_dialog()
{
    delete ui;
}

void grid_dialog::on_ok_clicked()
{
    //qDebug()<<m_label->getRoiRect().bottom();
    //qDebug()<<m_label->getRoiRect().top();
    //qDebug()<<m_label->getRoiRect().left();
    //qDebug()<<m_label->getRoiRect().right();

    if(m_label->getRoiRect().right()!=-1){
        data[0] = m_label->getRoiRect().left();
        data[1] = m_label->getRoiRect().right();
        data[2] = 500 - m_label->getRoiRect().bottom();
        data[3] = 500 - m_label->getRoiRect().top();
    }

    this->close();
}

void grid_dialog::on_cancel_clicked()
{
    this->close();
}
