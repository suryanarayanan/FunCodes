#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int dp[100][100][2];

int func(int n, int k, int lb)
{
    if(n == 0 && k == 0)
        return 1;
    else if(n == 0 && k)
        return 0;
    else if(k == 0)
    {
        int val = 0;
        if(lb == 0)
        {
            val = func(n-1, k, 1) + func(n-1, k, 0);
        }
        else
        {
            val = func(n-1, k, 0);
        }
        dp[n][k][lb] = val;
        return val;
    }

    if(dp[n][k][lb] != -1)
        return dp[n][k][lb];

    else
    {
        int val = 0;
        if(lb == 1)
            val = func(n-1, k-1, 1) + func(n-1, k, 0);
        else
            val = func(n-1, k, 0) + func(n-1, k, 1);
        dp[n][k][lb] = val;
        return val;
    }
}

int main()
{
    int d;
    for(int i = 0; i < 100; ++i)
        for(int j = 0 ; j<100; ++j)
            dp[i][j][0]=dp[i][j][1]=-1;
    cin>>d;
    while(d--)
    {
        int t, n, k;
        cin>>t>>n>>k;
        cout<< t<<" "<<func(n-1, k, 0) + func(n-1,k,1) << endl;
    }
    return 0;
}
