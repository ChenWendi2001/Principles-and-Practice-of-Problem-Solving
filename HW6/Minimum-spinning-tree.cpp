// Add any standard library if needed.
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
using namespace std;

// Implement your minimum spinning tree algorithm
int tmp ,n;

struct edge{
    int beg,end;
    int w;
    bool operator<(const edge&rp)const{
        return w>rp.w;
    }
};
class DisjointSet{
private:
    int size;
    int *parent;
public:
    DisjointSet(int n){
        size = n+1;
        parent = new int[size];
        for(int i = 0;i<size;++i) parent[i]=-1;
    }
    ~DisjointSet(){delete []parent;}
    void Union(int root1,int root2){
        if(root1==root2)return;
        if(parent[root1]>parent[root2]){
            parent[root2]+=parent[root1];
            parent[root1]=root2;
        }
        else{
            parent[root1]+=parent[root2];
            parent[root2]=root1;
        }
    }
    int Find(int x){
        if(parent[x]<0)return x;
        return parent[x]=Find(parent[x]);
    }
};
int kruskal(vector<int> v){
    int edgeAccepted = 0,ans=0,x,y;
    edge e;
    DisjointSet ds(n);
    priority_queue<edge> pq;
    for(int i = 0;i<n;i++){
        for(int j = i;j<n;j++){
            if(v[i*n+j]!=0){
                e.beg = i;
                e.end = j;
                e.w = v[i*n+j];
                pq.push(e);
            }
        }
    }
    while(edgeAccepted<n-1){
        e = pq.top();
        pq.pop();
        x = ds.Find(e.beg);
        y = ds.Find(e.end);
        if(x!=y){
            edgeAccepted++;
            ds.Union(x,y);
            ans+=e.w;
        }
    }
    return ans;
}


// Test your implementation
int main () {
    vector<int> v;
    while(cin>>tmp) {v.push_back(tmp);}
    n = sqrt(v.size());
    cout<<kruskal(v);
    return 0;
}