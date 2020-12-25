#include "lagrange.h"

double Lagrange::get_interp_x(double interp_x){
    int n = X.size();
    double ans = 0;
    for(int i = 0;i<n;i++){
        double s = Y[i];
        for(int j = 0;j<n;j++){
            if(j!=i)
                s *=(interp_x-X[j])/(X[i]-X[j]);
        }
        ans += s;
    }
    return ans;
}
