/*
*	The height of the Segment Tree is O(logN)
*	l: left	;	r: right
*	t: the Segment Tree
*	v: node
*	node [1:1] is a nodeLeaf
*
*
*/
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXN = 20005;
ll n, arr[MAXN], t[4*MAXN];
int q;
void build(long long int *a, int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = a[tl];
    } else {
        int tm = (tl + tr) / 2;
        build(a, v*2, tl, tm);
        build(a, v*2+1, tm+1, tr);
        t[v] = t[v*2] + t[v*2+1];
    }
}

void update(int v, int tl, int tr, int pos, int new_val) {
    if (tl == tr) {
        t[v] = new_val;
    } else {
        int tm = (tl + tr) / 2;
        if (pos <= tm)
            update(v*2, tl, tm, pos, new_val);
        else
            update(v*2+1, tm+1, tr, pos, new_val);
        t[v] = t[v*2] + t[v*2+1];
    }
}

/*query*/
int sum(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 0;
    if (l == tl && r == tr) {
        return t[v];
    }
    int tm = (tl + tr) / 2;
    return sum(v*2, tl, tm, l, min(r, tm)) \
           + sum(v*2+1, tm+1, tr, max(l, tm+1), r);
}

int main(){
	cin >> n >> q;
	for(int i = 0; i < n; i++){
		cin >> arr[i];
	}
	build(&arr[0],1,0,n-1); //create t[] with start from [0] to [n-1]
	while(q--)
	{
		int x, y, z;
		cin >> x >> y >> z;
		if(x == 1)
		{
			y--;
			update(1,0,n-1,y,z);
		}
		else
		{
			y--; z--;
			cout << sum(1, 0, n-1, y, z) << endl;
		}
	}
}
/*
*	TestCase:

input:

5 3
1 1 1 1 1 
1 3 2
2 2 2
2 3 4

output:

1
3

*/
