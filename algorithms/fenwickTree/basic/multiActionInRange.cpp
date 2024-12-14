/*
*	The height of the Fenwick Tree is O(logN)
*	l: left	;	r: right
*	t: the Segment Tree
*	v: node
*	node [1:1] is a nodeLeaf
*
*
*/
#include <iostream>
#include <bits/stdc++.h>
#include <sstream>

#define _DEBUG
#if defined(_DEBUG)
std::ostream& my_logger() {
    return std::cout;  // Log to console in debug mode
}
#else
    // No logs in non-debug mode
    // Define a no-op stream for when debugging is disabled
    class NullStream : public std::ostream {
    public:
        NullStream() : std::ostream(&null_buffer) {}
    private:
        class NullBuffer : public std::streambuf {
        public:
            int overflow(int c) override {
                return c; // Discard any characters
            }
        };
        NullBuffer null_buffer;
    };

    std::ostream& my_logger() {
        static NullStream null_stream;
        return null_stream;  // No-op stream when not debugging
    }
#endif

using namespace std;
using ll = long long;

int BIT[1000], a[1000], n;
void update(int pos, int val)
{
    my_logger() << " write value = " << val << " to:" << endl;
    for(; pos <= n; pos += pos &-pos)
    {        
        BIT[pos] += val;
        my_logger() << "BIT[" << pos << "]=" << BIT[pos] << "\t" ;
    }
    my_logger() <<  endl;

}
int query(int pos)
{
    int sum = 0;
    my_logger() <<  endl;
    my_logger() << "------------------------" << endl;
    my_logger() << " query start = " << pos << endl;
    for(; pos > 0; pos -= pos &(-pos)) //get position of bit 1 at end_place ex: 101(1) -> 10(1)0 -> (1)000 : 11 > 10 > 8
    {
        sum += BIT[pos];
        my_logger() << "+sum at pos:" << pos << "\t" ;
    }
    my_logger() <<  endl;
    my_logger() << "------------------------" << endl;
    return sum;
}

#define showArr(arr) showArrImpl(arr, #arr)
void showArrImpl(int arr[], const std::string& arr_name) {
    my_logger() << "show " << arr_name << "[]" << std::endl;
    for (int i = 0; i < n; i++) {
        my_logger() << arr[i] << "\t";
    }
    my_logger() << std::endl;
}

int main()
{
    cin >> n;
    int i;
    for(i = 1; i <= n; i++)
    {
        cin >> a[i];
        update(i, a[i]);
    }
    showArr(BIT);
    showArr(a);
    int result;
    result = query(10);
    cout << "sum of first 10 elements is " << result << endl;
    result = query(7) - query(2-1);
    cout << "sum of all elements in range [2, 7] is " << result << endl;
    return 0;
}
/*
*	TestCase:

input:
10
1 2 3 4 5 6 7 8 9 10

output:
sum of first 10 elements is 55
sum of all elements in range [2, 7] is 27

*/
