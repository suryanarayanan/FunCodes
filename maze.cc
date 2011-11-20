#include <iostream>
#include <fstream>
#include <cstdio>
#include <utility>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cmath>
#include <string>
#include <map>
#include <numeric>
#include <queue>
#include <stack>

using namespace std;

#define fz(i,n) for(int i = 0; i < n; ++i)
#define fl(i,l,n) for(int i = l; i < n; ++i)
#define mp(a,b) make_pair(a,b)
#define pb(a) push_back(a)
#define all(a) a.begin(),a.end()
#define sz(a) int(a.size())
#define lh(a) int(a.length())

vector <string> maze;

int bfs(int srcx, int srcy, int destx, int desty, vector< pair<int, int> >&path)
{
    map < pair<int,int> , pair<int,int> >parent;
    map< pair<int,int>, bool >flag;
    queue < pair< pair<int,int>, int> > q;
    q.push(mp(mp(srcx,srcy),0));
    parent[mp(srcx,srcy)] = mp(srcx,srcy);
    while(!q.empty())
    {
        pair<pair <int, int>, int >  top = q.front();
        int x = top.first.first;
        int y = top.first.second;
        int wt = top.second;
        q.pop();
        //    cout<<top.first.first<<" "<<top.first.second<<" "<<top.second<<endl;
        pair <int, int> temptop = mp(x,y);
        flag[temptop] = true;
        if(x == destx && y == desty)
        {
            while(!(parent[temptop].first == temptop.first && parent[temptop].second == temptop.second))
            {
//                cout<<temptop.first<<" "<<temptop.second<<endl;
                path.pb(temptop);
                temptop = parent[temptop];
            }
            return wt;
        }
        else
        {
            pair <int,int> temp1(x-1,y);
            pair <int,int> temp2(x,y-1);
            pair <int,int> temp3(x+1,y);
            pair <int,int> temp4(x,y+1);
            if(x > 0 && !flag[temp1] && maze[x-1][y] != 'x')
            {
                parent[temp1] = temptop;
                q.push(mp(temp1,wt+1));
                flag[temp1] = true;
            }
            if(y > 0 && !flag[temp2] && maze[x][y-1] != 'x')
            {
                q.push(mp(temp2,wt+1));
                flag[temp2] = true;
                parent[temp2] = temptop;
            }
            if(x < sz(maze)-1 && !flag[temp3] && maze[x+1][y] != 'x')
            {
                q.push(mp(temp3,wt+1));
                flag[temp3] = true;
                parent[temp3] = temptop;
            }
            if(y < lh(maze[0])-1 && !flag[temp4] && maze[x][y+1] != 'x')
            {
                q.push(mp(temp4,wt+1));
                flag[temp4] = true;
                parent[temp4] = temptop;
            }
        }
    }
    //this shud never happen
    return 9999;
}

int main()
{

    ifstream ifs("map.txt");
    string temp;
    while(ifs>>temp)
        maze.pb(temp);
    pair<int, int>a,b,c,d,e;
    fz(i,sz(maze))
    {
        fz(j,lh(maze[i]))
        {
            if(maze[i][j] == 'A')
                a = mp(i,j);
            if(maze[i][j] == 'B')
                b = mp(i,j);
            if(maze[i][j] == 'C')
                c = mp(i,j);
            if(maze[i][j] == 'D')
                d = mp(i,j);
            if(maze[i][j] == 'E')
                e = mp(i,j);
        }
    }

    vector <vector < vector<pair<int,int> > > >paths(6, vector< vector<pair<int,int> > >(6));
    vector < vector<int> > weight(6,vector<int>(6, 0));
    vector < pair<int,int> >look;
    look.pb(mp(35, 0));
    look.pb(a);
    look.pb(b);
    look.pb(c);
    look.pb(d);
    look.pb(e);


    fz(i,6)
    {
        fl(j,i+1,6)
        {
            int weigh = bfs(look[i].first, look[i].second, look[j].first, look[j].second, paths[j][i]);
            weight[i][j] = weight[j][i] = weigh;
            for(int k = sz(paths[j][i])-1; k >= 0 ; --k)
                paths[i][j].pb(paths[j][i][k]);
        }
    }
//printing all paths
/*
    fz(i,6)

    {
        fl(j,i+1,6)
        {
            cout<<"printing paths for "<<i<<" "<<j<<endl;
            fz(k,sz(paths[i][j]))
                cout<<paths[i][j][k].first<<" "<<paths[i][j][k].second<<endl;
            cout<<"\nweitgh :"<<weight[i][j]<<endl;
            cout<<"------------------------------"<<endl;
            cout<<endl;

        }
    }
*/

    vector <int> perm;
    fz(i,5)perm.pb(i);
    vector < pair<int, int> > result;
    int res = 9999999;
    do
    {
        vector < pair<int,int> >tpath;
        int total = weight[0][perm[0]];

        fz(i,sz(paths[0][perm[0]]))
           tpath.pb(paths[0][perm[0]][i]);
        fz(i,4)
        {
            total += weight[perm[i]][perm[i+1]];

            fz(l,sz(paths[perm[i]][perm[i+1]]))
                tpath.pb(paths[perm[i]][perm[i+1]][l]);

        }
        total += weight[0][perm[4]];
        fz(i,sz(paths[perm[4]][0]))
           tpath.pb(paths[perm[4]][0][i]);

        if(total < res)
        {
            res = total;
            result = tpath;
        }
    }while(next_permutation(all(perm)));
        cout<<"total cost: " << res<<endl;
        cout<<"0 35"<<endl;
        fz(i,sz(result))
        {
            cout<<result[i].second<<" "<<result[i].first<<endl;
        }
    return 0;
}
