#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;


// You can add additional standard libraries if necessary.
// Implement the Newton interpolation!
class Newton {
public:
    Newton(vector<double> x, vector<double> y): X(x), Y(y) {
        n = X.size();
        h = X[1]-X[0];
        for(int i = 0;i<n;i++){
            vector<double> a;
            for(int j = 0;j<n;j++){
                if(i==0) a.push_back(Y[j]);
                else a.push_back(0);
            }
            table.push_back(a);
        }
    }

    void init(){
        for(int i = 1;i<n;i++){
            for(int j = i;j<n;j++){
                table[i][j]=table[i-1][j]-table[i-1][j-1];
            }
        }
    }


    //debug code
    void print(){
        for(int i = 0;i<n;i++){
            for(int j = i;j<n;j++){
                cout<<table[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    double get_interp_x(double interp_x){
        double t = (interp_x-X[0])/h;
        double ans = Y[0];
        for(int i = 1;i<n;i++){
            double ans_k=table[i][i];
            for(int j = 1;j<=i;j++){
                ans_k/=1.0*j;
                ans_k*=1.0*(t-j+1);
            }
            ans+=ans_k;
        }
        return ans;
    }


private:
    vector<double> X, Y;
    vector<vector<double>> table;
    int n;
    double h;
};


// Test your implementation.
int main(int argc, const char * argv[]) {
    //  Input preprocessing.
    string str;
    getline(cin, str);
    stringstream xstr(str);
    getline(cin, str);
    stringstream ystr(str);
    
    // X and Y are two vectors of equal length to be traversed.
    vector<double> X, Y;
    double a;
    while (xstr >> a)
        X.push_back(a);
    while (ystr >> a)
        Y.push_back(a);
    
    // interp_x is the point to be interpolated.
    double interp_x;
    cin >> interp_x;

    // Do Newton interpolation for interp_x using X and Y, and print your results
    // Note: The result retains three decimal places (rounded)!

    Newton n(X,Y);
    n.init();
    //n.print();
    cout<<fixed<<setprecision(3)<<n.get_interp_x(interp_x);
    
    
    // End
    return 0;
}
