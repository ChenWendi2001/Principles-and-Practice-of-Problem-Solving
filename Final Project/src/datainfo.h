#ifndef DATAINFO_H
#define DATAINFO_H
#include<QString>
#include<QStringList>
#include<QDebug>
class dataInfo
{
private:


public:
    QString order_id;
    unsigned int dep_time;
    unsigned int end_time;
    double orig_lng;
    double orig_lat;
    double dest_lng;
    double dest_lat;
    double fee;

    int i;
    int j;
    int l_i;
    int l_j;

    int index_i;
    int index_j;

    int r_index_i;
    int r_index_j;

    dataInfo();
    dataInfo(QString str,unsigned int d_time,unsigned int e_time,double o_lng,
             double o_lat,double d_lng,double d_lat,double f);
    dataInfo(QString Str);
    bool operator<(const dataInfo&other)const;
};

#endif // DATAINFO_H
