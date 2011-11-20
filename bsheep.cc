#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <stack>
#include <cmath>
#include <cstdio>
using namespace std;

struct comp {
    int bottomx, bottomy;
    comp(int botx, int boty){bottomx = botx; bottomy = boty;}
    bool operator() (pair<int, pair<int,int> >i , pair<int, pair<int,int> >j)
    {
        int p1 = i.second.first - bottomx;
        int p2 = i.second.second - bottomy;
        int p3 = j.second.first - bottomx;
        int p4 = j.second.second - bottomy;
        if(p1 * p4 - p3*p2 == 0)
        {
            if(i.second.first == j.second.first && i.second.second == j.second.second)
            {
                return j.first < i.first;
            }
            else
            {
                return (p3*p3 + p4*p4) > (p1*p1 + p2*p2);
            }
        }
        if(p1 * p4 - p3*p2 > 0)
            return true;
        else
            return false;
    }
};

int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        int n;
        cin>>n;
        vector <pair <int, int> > sheep;
        pair <int, int> bottom(9999999, 9999999);
        int bottom_indx = 0;
        for(int i = 0; i< n; ++i)
        {
            int a,b;
            cin >>a>>b;
            sheep.push_back(make_pair(a,b));
            if(b < bottom.second)
            {bottom.second = b,bottom.first = a, bottom_indx = i;}
            else if(b == bottom.second && a < bottom.first)
            {bottom.second = b, bottom.first = a, bottom_indx = i;}
        }
        vector < pair<int, pair <int,int> > > sheep1;
        for(int i = 0 ; i< n; ++i)
        {
            if(sheep[i].first == bottom.first && sheep[i].second == bottom.second)
            {
                continue;
            }
            else
            {
                sheep1.push_back(make_pair(i,sheep[i]));
            }
        }

            stable_sort(sheep1.begin(), sheep1.end(), comp(bottom.first, bottom.second));
             // for(int i =0; i< int(sheep1.size()); ++i)
             // {
             //     cout<<sheep1[i].first<<" ";
             //     cout<<double(sheep1[i].second.second - bottom.second)/double(sheep1[i].second.first - bottom.first)<<" ";
             //     cout<<endl;
             // }
             // cout<<endl;
            stack < int > S;
            S.push(bottom_indx);
            for(int i =0 ; i< int(sheep1.size()); ++i)
            {
                int x1,x2,y1,y2, x3 = sheep1[i].second.first,y3 = sheep1[i].second.second;
                int x2ind = 0;
                if(!S.empty())
                {
                    if(x3 == sheep[S.top()].first && y3 == sheep[S.top()].second && sheep1[i].first > S.top())
                        continue;
                }
                if(S.size() < 2)
                    S.push(sheep1[i].first);
                else
                {
                    x2 = sheep[S.top()].first;
                    y2 = sheep[S.top()].second;
                    x2ind = S.top();
                    S.pop();
                    x1 = sheep[S.top()].first;
                    y1 = sheep[S.top()].second;
                    if((x2 - x1)*(y3 - y1) > (y2 - y1)*(x3- x1))
                    {
                        S.push(x2ind);
                        S.push(sheep1[i].first);
                    }
                    else
                    {
                        do
                        {
                            if(S.size() < 2)
                            {
                                S.push(sheep1[i].first);
                                break;
                            }
                            int x2 = sheep[S.top()].first, y2 = sheep[S.top()].second;
                            int x2ind = S.top();
                            S.pop();
                            int x1 = sheep[S.top()].first, y1 = sheep[S.top()].second;
                            if((x2 - x1)*(y3 - y1) > (y2 - y1)*(x3- x1))
                            {
                                S.push(x2ind);
                                S.push(sheep1[i].first);
                                break;
                            }
                        }while(1);
                    }
                }
            }
            vector<int> result;
            double distance = 0.;
            while(!S.empty())
            {
                result.push_back(S.top());
                S.pop();
            }
            for(int i = 0; i< int(result.size()); ++i)
            {
                int param1 = (sheep[result[i]].first - sheep[result[(i+1)%int(result.size())]].first);
                int param2 = (sheep[result[i]].second - sheep[result[(i+1)%int(result.size())]].second);
                distance += sqrt(double(param1*param1 + param2*param2));
            }
            printf("%.2lf\n", distance);
            for(int i=int(result.size())-1; i >= 0; --i)
            {
                if(i!=int(result.size())-1)
                    cout<<" ";
                printf("%d", result[i]+1);
            }
            printf("\n");
            if(t)
                printf("\n");
    }
    return 0;
}
