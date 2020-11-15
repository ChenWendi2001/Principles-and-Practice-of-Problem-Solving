#include <iostream>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <cmath>
#include <queue>
#include <vector>
using namespace std;

const int population = 20;
const int max_iteration_num = 1000000;
const double mutation_rate = 0.1;
int count = 0;
int iteration = 0;
double ans_X;
double my_min = 1000000.0;
bitset<16> individual[population];
vector<bitset<16>> group;

double decode(bitset<16> s){
    double f=-1;
    for(int i =0;i<16;++i){
        if(s[i]) f += pow(2,(3-i));
    }
    return f;
}

double func(double x){
    return x*sin(x);
}

struct info{
    bitset<16> b;
    double x;
    double v;
    info(bitset<16> s){
        b = s;
        x = decode(s);
        v = func(x);
    }
    bool operator<(const info& other)const{
        return v>other.v;
    }
    void operator=(const info&other){
        b = other.b;
        x = other.x;
        v = other.v;
    }
};

int my_rand(){
    return rand()%2;
}

void init(){
    srand(time(0));
    for(int i = 0;i<population;++i){
        for(int j = 0;j<16;j++){
            individual[i][j] = my_rand();
        }
    }
}

bool selection(){
    priority_queue<info> q;
    for(int i = 0;i<group.size();++i){
        q.push(info(group[i]));
    }
    vector<bitset<16>> v;
    double min,x;
    for(int i = 0;i<group.size();++i){
        info in = q.top();
        min = in.v;
        x = in.x;
        if(i ==0){

            cout<<"iteration"<<iteration<<":";
            cout<<"x="<<in.x<<" y="<<in.v<<endl;
        }
        for(int j =0;j<group.size()-i;++j){
            v.push_back(in.b);
        }
        q.pop();
    }

    for(int i = 0;i<population;++i){
        individual[i] = v[rand()%(v.size())];
    }
    group.clear();
    if(min-my_min<0){
        my_min = min;
        ans_X = x;
        return true;
    }
    else{
        return false;
    }
}

void crossover(){
    vector<bitset<16>> v;
    for(int i = 0;i<population;++i){
        v.push_back(individual[i]);
    }
    for(int i = 0;i<population/2;++i){
        int pos = 15-rand()%16;
        int rand1 = rand()%(v.size());
        int rand2 = rand()%(v.size());
        while(rand2==rand1) rand2 = rand()%(v.size());
        group.push_back(v[rand1]);
        group.push_back(v[rand2]);
        for(int j = pos;j<16;j++){
            int temp = v[rand1][j];
            v[rand1][j] = v[rand2][j];
            v[rand2][j] = temp;
        }
        group.push_back(v[rand1]);
        group.push_back(v[rand2]);
        v.erase(v.begin()+rand1);
        if(rand1<rand2) v.erase(v.begin()+rand2-1);
        else v.erase(v.begin()+rand2);
    }
}

void mutation(){
    for(int i = 0;i<population;++i){
        if(rand()%10<mutation_rate*10){
            int pos = rand()%16;
            while(individual[i][pos]==1 && pos>0){
                individual[i][pos]=0;
                pos--;
            }
            individual[i][pos]=1;
            group.push_back(individual[i]);
        }
        if(rand()%10<mutation_rate*10){
            int pos = rand()%16;
            while(individual[i][pos]==0 && pos>0){
                individual[i][pos]=1;
                pos--;
            }
            individual[i][pos]=0;
            group.push_back(individual[i]);
        }
    }
}

int main() {
    init();
    for(int i =0;i<max_iteration_num;++i){
        iteration++;
        crossover();
        mutation();
        bool flag = selection();
        if(flag){
            count = 0;
        }
        else{
            count++;
        }
        if(count>100 && iteration>5000){
            cout<<"The answer is x="<<ans_X<<endl;
            return 0;
        }
    }
    return 0;
}
