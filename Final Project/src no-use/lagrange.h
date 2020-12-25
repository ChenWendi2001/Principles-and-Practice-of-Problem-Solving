#ifndef LAGRANGE_H
#define LAGRANGE_H
#include <QVector>

class Lagrange {
public:
    Lagrange(QVector<double> x, QVector<double> y): X(x), Y(y) {}

    double get_interp_x(double interp_x);

private:
    QVector<double> X, Y;
};

#endif // LAGRANGE_H
