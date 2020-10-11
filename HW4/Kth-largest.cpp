#include <iostream>
#include <vector>
using namespace std;

// Implement your selection algorithm. You can add any standard library if needed.
//

int kth(vector<int> &v,int left,int right,int k){
    int temp1 = left;
    int temp2 = right;
    int p = v[left];

    do {
        while (left < right && v[right] <= p) --right;
        if (left < right) { v[left] = v[right]; ++left; }
        while (left < right && v[left] >= p) ++left;
        if (left < right) { v[right] = v[left]; --right; }
    } while (left != right);

    if(left == k) return p;
    if(left>k) return kth(v,temp1,left-1,k);
    else return kth(v,left+1,temp2,k);

}



int main() {
    // Input processing.
    int in;
    vector<int> v;

    //int t = 7;
    while(cin>>in){
        //cin>>in;
        v.push_back(in);
    }
    
    int k = v[v.size()-1];
    v.pop_back();
    // select the target and print the results.
    //cout<<v.size()<<endl;
    cout<<kth(v,0,v.size()-1,k-1);
    return 0;
}
