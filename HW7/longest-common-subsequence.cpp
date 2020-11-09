#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;

int max(int x,int y){
    return x<y? y:x;
}

void dfs(int **dp,int x,int y,string X,string Y,string cur,set<string> &s){
    if(dp[x][y]==0){
        if(!s.count(cur)) s.insert(cur);
        return;
    }
    if(X[x-1]==Y[y-1]){
        dfs(dp,x-1,y-1,X,Y,X[x-1]+cur,s);
    }
    else{
        if(dp[x-1][y]==dp[x][y]){
            dfs(dp,x-1,y,X,Y,cur,s);
        }
        if(dp[x][y-1]==dp[x][y]){
            dfs(dp,x,y-1,X,Y,cur,s);
        }
    }
}

set<string> LCS(string X, string Y)
{
    set<string> ans;
    int len1 = X.size();
    int len2 = Y.size();
    int** dp = new int*[len1+1];
    for(int i = 0;i<=len1;++i){
        dp[i] = new int[len2+1];
        for(int j = 0;j<=len2;j++){
            dp[i][j]=0;
        }
    }
    for(int i = 1;i<=len1;++i){
        for(int j = 1;j<=len2;++j){
            if(X[i-1]==Y[j-1]) dp[i][j] = dp[i-1][j-1]+1;
            else{
                dp[i][j] = max(dp[i-1][j],dp[i][j-1]);
            }
        }
    }

    string null = "";

    dfs(dp,len1,len2,X,Y,null,ans);

    for(int i = 0;i<=len1;++i){
        delete []dp[i];
    }
    delete []dp;

    return ans;
}

int main()
{
    string X, Y;
    cin >> X;
    cin >> Y;

    set<string> lcs = LCS(X, Y);

    // print set elements
    for (string str : lcs)
        cout << str << endl;

    return 0;
}