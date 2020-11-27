#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init_UI();
    Init_Signal_Slots();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init_UI()
{
    setWindowTitle("Visualizer V1.0");

    QMenu* menu = ui->menuBar->addMenu(tr("&Open"));
    ui->mainToolBar->setVisible(false);
    menu->addAction(tr("Select a File"));

    widget = new QWidget;
    outerLayout = new QVBoxLayout(widget);
    /*
    QFileDialog *fileDialog = new QFileDialog(this);
    outerLayout -> addWidget(fileDialog);
    fileDialog->setWindowTitle(QString("Please select your file:"));
    */
    topLayout = new QHBoxLayout();
    QLabel* fileLable = new QLabel("Please select your file:");
    fileLable->setFixedWidth(150);
    topLayout->addWidget(fileLable);
    //topLayout->addSpacerItem(new QSpacerItem(200,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    directoryComboBox = new QComboBox;
    topLayout->addWidget(directoryComboBox);
    directoryComboBox->setMinimumWidth(350);
    directoryComboBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    select = new QPushButton("select");
    select->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    topLayout->addWidget(select);

    secondLayout = new QHBoxLayout();
    startHour = new QComboBox;
    endHour = new QComboBox;
    for(int i = 0;i<24;++i){startHour->addItem(QString::number(i));endHour->addItem(QString::number(i));}
    QLabel* timeLable = new QLabel("Please select time(h):");
    timeLable->setFixedWidth(150);
    secondLayout->addWidget(timeLable);
    secondLayout->addWidget(startHour);
    QLabel * to = new QLabel(" ~ ");
    to->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    secondLayout->addWidget(to);
    secondLayout->addWidget(endHour);

    thirdLayout = new QHBoxLayout();
    start = new QPushButton("Start");
    start->setEnabled(false);
    thirdLayout->addWidget(start);

    chart = new QChart();
    chartview = new QChartView(chart);
    chartview->setMinimumHeight(300);

    axisX = nullptr;
    axisY = nullptr;

    outerLayout ->addLayout(topLayout);
    outerLayout ->addLayout(secondLayout);
    outerLayout ->addLayout(thirdLayout);
    outerLayout ->addWidget(chartview);
    widget->setLayout(outerLayout);
    setCentralWidget(widget);


    ui->statusBar->showMessage (tr("Select file and time."));


}
void MainWindow::Select_File()
{
    QString directory=
            QDir::toNativeSeparators(QFileDialog::getOpenFileName(this,tr("Select a File"),QDir::currentPath(),"Data (*.csv);;"));
    if(!directory.isEmpty()){
        if(directoryComboBox->findText(directory)==-1)
            directoryComboBox->addItem(directory);
        directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    }
}

void MainWindow::Assert_Validity()
{
    if(directoryComboBox->count() == 0 ){
        start->setEnabled(false);
        ui->statusBar->showMessage (tr("Select file and time."));
    }
    else if(startHour->currentIndex()>=endHour->currentIndex()){
        start->setEnabled(false);
        ui->statusBar->showMessage (tr("Starting time should be ealier than ending time."));
    }
    else{
        start->setEnabled(true);
        ui->statusBar->showMessage (tr("Everything is ready. Start as you want."));
    }
}

void MainWindow::Init_Signal_Slots()
{
    connect(select,SIGNAL(clicked(bool)),this,SLOT(Select_File()));
    connect(directoryComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(Assert_Validity()));
    connect(startHour,SIGNAL(currentIndexChanged(int)),this,SLOT(Assert_Validity()));
    connect(endHour,SIGNAL(currentIndexChanged(int)),this,SLOT(Assert_Validity()));
    connect(directoryComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(Load_Data()));
    connect(startHour,SIGNAL(currentIndexChanged(int)),this,SLOT(Load_Data()));
    connect(endHour,SIGNAL(currentIndexChanged(int)),this,SLOT(Load_Data()));
    connect(start,SIGNAL(clicked(bool)),this,SLOT(Update_Data()));
    connect(ui->menuBar,SIGNAL(triggered(QAction*)),this,SLOT(Select_File()));
}

void MainWindow::Load_Data()
{
    if(directoryComboBox->count()!=0){
        QString filename = directoryComboBox->currentText();
        QDir dir = QDir::current();
        QFile file(dir.filePath(filename));
        file.open(QIODevice::ReadOnly);
        QTextStream * out = new QTextStream(&file);
        QStringList pair = out->readAll().split("\n");
        data.clear();
        for(int i = (startHour->currentText()).toInt()+1;i<=(endHour->currentText()).toInt()+1;++i){
            QStringList temp = pair.at(i).split(",");
            data.push_back(std::make_pair(temp.at(0).toInt(),temp.at(1).toInt()));
        }
        file.close();
    }
}

void MainWindow::Update_Data()
{
    ui->statusBar->showMessage (tr("Great! You've drawn a curve."));
    series = new QLineSeries();
    series->setVisible(true);
    series->setName("Real-time Taxi Demand");
    for (std::pair<int,int>p:data){
        series->append(p.first,p.second);
    }
    chart->removeAllSeries();
    if(axisX != nullptr)chart->removeAxis(axisX);
    if(axisY != nullptr)chart->removeAxis(axisY);
    chart->addSeries(series);
    axisX = new QValueAxis;
    axisY = new QValueAxis;
    axisY->setTitleText("Demand");
    axisX->setTitleText("Time(h)");
    chart->setAxisX(axisX,series);
    chart->setAxisY(axisY,series);
    //chartview->update();
}


