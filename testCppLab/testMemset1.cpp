#include <iostream>
#include <cstring>
#include <bitset>


using namespace std;
char char_arr[100];
int arr[100];
int main() {
    memset(arr, 0, sizeof (arr)); //True
    memset(arr, -1, sizeof (arr)); //True
    memset(char_arr, 'c', sizeof (char_arr)); //True
    memset(arr, 1, sizeof (arr)); //False

    // check:
    cout << bitset<32> (arr[0]) << " arr[0]= " << arr[0] << "\n";
    // result: 00000001000000010000000100000001 arr[0]= 16843009

    return 0;
}


