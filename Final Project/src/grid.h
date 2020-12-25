#ifndef GRID_H
#define GRID_H

#include <QVector>
#include <QString>
#include <QStringList>

class Grid
{
private:
    int grid_id;
    QVector<double> lat;
    QVector<double> lng;
public:
    Grid();
    Grid(QString str);
};

#endif // GRID_H
