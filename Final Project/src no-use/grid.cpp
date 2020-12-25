#include "grid.h"

Grid::Grid()
{

}

Grid::Grid(QString str)
{
    QStringList list = str.split(",");
    grid_id = list[0].toInt();
    for(int i = 0;i<4;++i){
        lat.push_back(list[2*i+1].toDouble());
        lng.push_back(list[2*i+2].toDouble());
        //qDebug()<<list[2*i+1].toDouble()<<" "<<list[2*i+2].toDouble();
    }
}
