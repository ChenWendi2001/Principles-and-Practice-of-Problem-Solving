#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int min(int a, int b, int c){
        if(a<=b&&a<=c) return a;
        else if(b<=a && b<=c) return b;
        return c;
    }

    int nthUglyNumber(int n) {
        int a = 0,b = 0,c = 0;
        vector<int> ugly_number;
        ugly_number.push_back(1);
        for(int i = 0;i<n;i++){
            int x = ugly_number[a]*2;
            int y = ugly_number[b]*3;
            int z = ugly_number[c]*5;
            ugly_number.push_back(min(x,y,z));
            if(x<=y&&x<=z)
                a+=1;
            if(y<=x&&y<=z)
                b+=1;
            if(z<=x&&z<=y)
                c+=1;
        }
        return ugly_number[n-1];
    }
};

int main(){
	Solution s;
	int n;
	cin >> n;
	cout << s.nthUglyNumber(n) << endl;
	return 0;
}