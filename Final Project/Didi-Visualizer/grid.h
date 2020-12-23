#ifndef GRID_H
#define GRID_H

#include <QVector>
#include <QString>
#include <QStringList>
#include <QDebug>

class Grid
{
private:
    int grid_id;
public:
    QVector<double> lat;
    QVector<double> lng;
    Grid();
    Grid(QString str);
};

#endif // GRID_H
