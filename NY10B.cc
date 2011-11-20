#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> to_str(int x, int b)
{
    vector <int> to_return;
    while(x > 0)
    {
        to_return.push_back(x%b);
        x/=b;
    }
    return to_return;
}

vector<int> sumb(vector <int>& a, vector<int>& b, int nim)
{
    vector <int> res;
    vector <int> c,d;
    if(int(a.size()) >= int(b.size()))
    {        c = a; d = b;}
    else
    { c = b; d = a;}
    for(int i = 0; i < int(d.size()); ++i)
    {
        res.push_back((c[i] + d[i])%nim);
    }
    for(int i = d.size(); i < int(c.size()); ++i)
    {
        res.push_back((c[i])%nim);
    }
    return res;
}

int to_dec(vector<int>& a, int nim)
{
    int ans = 0;
    long long cnt = 1;
    for(int i =0; i< int(a.size()); ++i)
    {
        ans += a[i] * cnt;
        cnt*=nim;
    }
    return ans;
}

int main()
{
    int t,b,x,y;
    int n;
    cin>>n;
    while(n--)
    {
        cin>>t>>b>>x>>y;
        vector<int> xstr = to_str(x,b);
        vector<int> ystr = to_str(y,b);
        vector<int> sum = sumb(xstr, ystr, b);
        cout<<t<<" "<<to_dec(sum,b)<<endl;
    }
    return 0;
}
