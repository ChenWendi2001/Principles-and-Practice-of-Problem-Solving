#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Implement your Jacobi Iteration algorithm.
// You can add any standard library if needed.
//

const double ZERO = 1e-10;

int get_N(vector<double> v)
{
    int len = v.size();
    int i = 1;
    while (i * (i + 1) != len)
    {
        i++;
    }
    return i;
}

void my_swap(double **a, double *b, int i, int j, int n)
{
    for (int k = 0; k < n; k++)
    {
        double temp = a[i][k];
        a[i][k] = a[j][k];
        a[j][k] = temp;
    }

    double temp = b[i];
    b[i] = b[j];
    b[j] = temp;
}

double myabs(double x)
{
    return x < 0 ? -x : x;
}

bool isZero(double x)
{
    return myabs(x) < ZERO;
}

// Test your implementation.
int main()
{
    // Input processing.
    vector<double> coin;
    double in;

    //int t = 12;
    while (cin >> in)
    {

        coin.push_back(in);
    }

    int n = get_N(coin);
    int index = 0;

    double **A = new double *[n];
    double *B = new double[n];
    double *x = new double[n];

    for (int i = 0; i < n; i++)
    {
        A[i] = new double[n];
        for (int j = 0; j < n; j++)
        {
            A[i][j] = coin[index];
            index++;
        }
        B[i] = coin[index];
        index++;
    }
    for (int q = 0; q < 10; q++)
    {
        for (int i = 0; i < n; i++)
            x[i] = q;

        // Solve the linear system and print the results.
        int i, j, k;
        double tmp;
        double *x2 = new double[n];
        for (k = 0; k < 1000000; k++)
        {
            for (i = 0; i < n; i++)
            {
                x2[i] = x[i];
            }
            for (i = 0; i < n; i++)
            {
                tmp = 0.0;
                for (j = 0; j < n; j++)
                {
                    if (j == i)
                        continue;
                    tmp += A[i][j] * x2[j];
                }
                x[i] = (B[i] - tmp) / A[i][i];
            }

            for (i = 0, j = 0; i < n; i++)
                if (fabs(x2[i] - x[i]) < 0.00001)
                    j++;

            if (j == n)
            {
                for (int i = 0; i < n; i++)
                {
                    cout << fixed << setprecision(3) << x[i] << " ";
                }
                return 0;
            }
        }
    }

    for (int k = 0; k < n; k++)
    {
        if (isZero(A[k][k]))
        {
            int i;
            for (i = k + 1; i < n; i++)
            {
                if (A[i][k])
                {
                    my_swap(A, B, i, k, n);
                    break;
                }
            }
            if (i == n)
            {
                continue;
            }
        }
        for (int i = k + 1; i < n; i++)
        {
            double temp = A[i][k] / A[k][k];
            for (int j = k; j < n; j++)
            {
                A[i][j] = A[i][j] - temp * A[k][j];
            }
            B[i] = B[i] - temp * B[k];
        }

        for (int i = k - 1; i >= 0; i--)
        {
            double temp = A[i][k] / A[k][k];
            for (int j = k; j < n; j++)
            {
                A[i][j] = A[i][j] - temp * A[k][j];
            }
            B[i] = B[i] - temp * B[k];
        }
    }

    bool flag = false;

    for (int k = n - 1; k >= 0; k--)
    {
        if (isZero(A[k][k]))
        {

            if (isZero(B[k]))
            {
                cout << "No unique solution!";
                return 0;
            }
            else
            {
                flag = true;
            }
        }
        else if (!flag)
        {
            x[k] = B[k] / A[k][k];
        }
    }

    if (flag)
    {
        cout << "No solution!";
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        cout << fixed << setprecision(3) << x[i] << " ";
    }

    return 0;
}
