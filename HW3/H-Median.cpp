#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;

class Solution
{
public:
    int divide(vector<int> &a, int low, int high)
    {
        int k = a[low];
        do
        {
            while (low < high && a[high] <= k)
                --high;
            if (low < high)
            {
                a[low] = a[high];
                ++low;
            }
            while (low < high && a[low] >= k)
                ++low;
            if (low < high)
            {
                a[high] = a[low];
                --high;
            }
        } while (low != high);
        a[low] = k;
        return low;
    }

    void quickSort(vector<int> &a, int low, int high)
    {
        if (low >= high)
            return;
        int mid = divide(a, low, high);
        quickSort(a, low, mid - 1);
        quickSort(a, mid + 1, high);
    }


    float hMedian(vector<int> &citations)
    {
        quickSort(citations,0,citations.size()-1);
        //for(int i = 0;i<citations.size();++i) cout<< citations[i]<<' ';
        //code here!!
        int i;
        for(i = 1;i<citations.size()+1;i++){
            if(i>=citations[i-1]){
                break;
            } 
        }
        if(i!=citations[i-1])i--;
        double ans;
        //cout<<i<<endl;
        if(i%2==0){
            ans = (citations[i/2]+ citations[i/2-1])/2.0;
        }
        else{
            ans = citations[i/2];
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
