#include <iostream>
#include <vector>
#include <climits>
#include <functional>
#define NDEBUG (1)
#include "debug_macro.h"
using namespace std;

bool init();
#define RETURN_FALSE 0
#define RETURN_TRUE 1
#define CONTINUE 2
#define BREAK 3
#define GOTO 4

#define DO_STH(action, state, label) { if(action()) \
    switch (state) {                                \
    case RETURN_FALSE: return false;                \
    case RETURN_TRUE: return true;                  \
    case CONTINUE: goto _continue##label;           \
    case BREAK: goto _break##label;                 \
    }                                               \
}

vector<function<bool()>> optCtlCall;
vector<function<bool()>> callbackDoSthList;

struct dosthPackage_t {
    bool (*callback)();  // Vector to store callback actions
    int jumpState;
};

vector<dosthPackage_t> dosthList;

/* extend code section push it to 1 file*/
int val = 0;
bool isCall(){
    if (val < 10) return true;
    return false;
};
bool isHello() {
    cout << "hello\n";
    return true;
};
/* extend code section */

bool init(){
    cout << "init list\n";
    for(int i=0; i < 20;) {
    _continueWhile1:    //template 1
        val=i;
        i++;
        cout << "val1 = " << val << "\n";
        DO_STH(dosthList[0].callback,dosthList[0].jumpState,While1);    //template in while
        cout << "val2 = " << val << "\n";
        if (val > 20){
            cout << endl;
            break;
        }
    }
    _breakWhile1:
    return false;
}


int main() {
    struct dosthPackage_t A;
    A.callback = isCall;
    A.jumpState = CONTINUE;
    dosthList.push_back(A);

    callbackDoSthList.push_back(isCall);

    if(init()) {
        cout << "init success\n";
    } else {
        cout << "init failed\n";
    }
    return 0;
}
