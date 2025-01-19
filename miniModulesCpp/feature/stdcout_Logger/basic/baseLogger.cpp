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

int my_arr[1000], a[1000], n;

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
        my_arr[i] = a[i];
    }
    showArr(my_arr);
    showArr(a);

    return 0;
}
/*

*/
