#include "datainfo.h"

dataInfo::dataInfo()
{

}

dataInfo::dataInfo(QString str, unsigned int d_time, unsigned int e_time,
                   double o_lng, double o_lat, double d_lng, double d_lat,double f)
{
    order_id = str;
    dep_time = d_time;
    end_time = e_time;
    orig_lng = o_lng;
    orig_lat = o_lat;
    dest_lng = d_lng;
    dest_lat = d_lat;
    fee = f;

}

//dataInfo::dataInfo(QString str)
//{
//    QStringList list = str.split(",");
//    order_id = list[0];
//    dep_time = list[1].toInt();
//    end_time = list[2].toInt();
//    orig_lng = list[3].toDouble();
//    orig_lat = list[4].toDouble();
//    dest_lng = list[5].toDouble();
//    dest_lat = list[6].toDouble();
//    fee = list[7].toDouble();

//    i = (orig_lng - 103.908407474531)/(104.222044525468-103.908407474531)*10;
//    j = (orig_lat - 30.524081949676)/(30.7938780503239-30.524081949676)*10;

//    l_i = (dest_lng - 103.908407474531)/(104.222044525468-103.908407474531)*10;
//    l_j = (dest_lat - 30.524081949676)/(30.7938780503239-30.524081949676)*10;

//    index_i = (orig_lng - 103.908407474531)/(104.222044525468-103.908407474531)*500;
//    index_j = (orig_lat - 30.524081949676)/(30.7938780503239-30.524081949676)*500;
//    index_j = 499-index_j;

//    r_index_i = (orig_lng - 103.908407474531)/(104.222044525468-103.908407474531)*100;
//    r_index_j = (orig_lat - 30.524081949676)/(30.7938780503239-30.524081949676)*100;
//    r_index_j = 99-r_index_j;

//    //qDebug()<<i;
//    //qDebug()<<j;

//}

dataInfo::dataInfo(QString str, double start_x, double end_x, double start_y, double end_y)
{
    QStringList list = str.split(",");
    order_id = list[0];
    dep_time = list[1].toInt();
    end_time = list[2].toInt();
    orig_lng = list[3].toDouble();
    orig_lat = list[4].toDouble();
    dest_lng = list[5].toDouble();
    dest_lat = list[6].toDouble();
    fee = list[7].toDouble();

    i = (orig_lng - start_x)/(end_x-start_x)*10;
    j = (orig_lat - start_y)/(end_y-start_y)*10;

    l_i = (dest_lng - start_x)/(end_x-start_x)*10;
    l_j = (dest_lat - start_y)/(end_y-start_y)*10;

    index_i = (orig_lng - start_x)/(end_x-start_x)*500;
    index_j = (orig_lat - start_y)/(end_y-start_y)*500;
    index_j = 499-index_j;

    r_index_i = (orig_lng - start_x)/(end_x-start_x)*100;
    r_index_j = (orig_lat - start_y)/(end_y-start_y)*100;
    r_index_j = 99-r_index_j;
}

bool dataInfo::operator<(const dataInfo&other)const
{
    return dep_time<other.dep_time;
}


