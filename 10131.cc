#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int w1,s1;
    vector <int> w;
    vector <int> s;
    int n=0;
    while(cin>>w1)
    {
        cin>>s1;
        w.push_back(w1);
        s.push_back(s1);
        n++;
    }

    vector <int> dp(n,1);
    vector <int> prev(n,-1);

    for(int i = 0; i <n; ++i)
    {
        prev[i] = i;
        for(int j = i-1; j >= 0 ; --j)
        {
            if(dp[j] + 1 > dp[i] && w[i] > w[j] && s[i] < s[j])
            {
                dp[i] = dp[j]+1;
                prev[i] = j;
            }
        }
    }

    int big = -1;
    int ind = 0;
    for(int i =0 ; i< n; ++i)
    {
        if(dp[i] > big)
        {
            big = dp[i];
            ind = i;
        }
    }
    vector <int> ans;
    cout<<big<<endl;
    while(prev[ind] != ind)
    {
        ans.push_back(ind);
        ind = prev[ind];
    }
    ans.push_back(ind);
    for(int i = int(ans.size())-1; i >= 0 ; --i)
        cout<<ans[i]+1<<endl;
    return 0;
}
