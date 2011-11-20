#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

void find_prime(vector <bool>& prime)
{
    prime[0]=prime[1] = false;
    for(int i = 2; i*i <= 65000; ++i)
    {
        if(prime[i] == false)
            continue;
        for(int j = 2 * i; j <= 65000; j+=i)
        {
            prime[j] = false;
        }
    }
    return;
}

int func(long long a, int n, int m)
{
    long long val = 0;
    if( n <= 0)
        return 1;
    else
    {
        val = func((a%m*a%m)%m, n/2, m);
        if(n%2)
            val = (val%m*a%m)%m;
    }
    return val;
}


int main()
{
    long long n;
    vector <bool> prime(65001,true);
    find_prime(prime);
    while(1)
    {
        cin>>n;
        if(!n)
            break;
        if(prime[n])
        {
            cout<<n<<" is normal."<<endl;
        }
        else
        {
             int cnt = 0;
             for(int i = 2; i < n; ++i)
             {
                 if(func(i,n,n) == i)
                 {
                    ++cnt;
                 }
                 else
                     break;
             }
             if(cnt == n-2)
             {
                 cout<<"The number "<<n<<" is a Carmichael number."<<endl;
             }
             else
             {
                 cout<<n<<" is normal."<<endl;
             }
        }
    }
    return 0;
}
