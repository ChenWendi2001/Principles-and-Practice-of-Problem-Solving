#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;


template<class T>
class pQueue{
    T data[300001];

    void percolateDown(int hole){
        int child;
        T temp = data[hole];
        for(;hole*2<=size;hole=child){
            child = hole*2;
            if(child!=size&&data[child+1]>data[child])
                child++;
            if(data[child]>temp) data[hole]=data[child];
            else break;
        }
        data[hole]= temp;
    }

public:
    int size;
    pQueue():size(0){}
    bool isEmpty(){return size==0;}
    void enQueue(T x){
        int hole = ++size;
        for(;hole>1&&x>data[hole/2];hole/=2) data[hole]=data[hole/2];
        data[hole]=x;
    }

    T deQueue(){
        T minItem;
        minItem = data[1];
        data[1]=data[size--];
        percolateDown(1);
        return minItem;
    }
    pQueue(vector<T> a,int n){
        size=n;
        for(int i = 0;i<n;i++) data[i+1]=a[i];
        for(int i = size/2;i>0;i--){
            percolateDown(i);
        }
    }

};


class Solution
{
public:

    float hMedian(vector<int> &citations)
    {
        
        //for(int i = 0;i<citations.size();++i) cout<< citations[i]<<' ';
        //code here!!
        pQueue<int> p(citations,citations.size());
        vector<int> v;
        int i;
        for(i = 1;i<citations.size()+1;i++){
            v.push_back(p.deQueue());
            if(i>=v[i-1]) break;
        }
        if(i!=v[i-1])i--;
        double ans;
        //cout<<i<<endl;
        if(i%2==0){
            ans = (v[i/2]+ v[i/2-1])/2.0;
        }
        else{
            ans = v[i/2];
        }
        return ans;
    }
};

int main(int argc, char **argv)
{
    std::string line;
    int number;
    std::vector<int> numbers;

    std::getline(std::cin, line);
    std::istringstream stream(line);
    while (stream >> number)
        numbers.push_back(number);

    Solution s;
    float res = s.hMedian(numbers);

    std::cout << std::fixed;
    std::cout << std::setprecision(1) << res << endl;

    return 0;
}
