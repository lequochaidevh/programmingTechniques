#include <iostream>
#include <vector>
#include <climits>
#include <functional>
using namespace std;

// Macros to simulate control flow actions
#define RETURN_TRUE []() { return true; }
#define RETURN_FALSE []() { return false; }
#define CONTINUE_ACTION [&, continue_flag = false]() mutable { continue_flag = true; return continue_flag; }
#define CONTINUE_M continue;
vector<function<bool()>> optCtlCall;  // Vector to store control flow actions
vector<function<bool()>> callbackDoSthList;  // Vector to store callback actions

int val = 0;
bool isCall(){
    if (val < 10) return true;
    return false;
}

bool init(){
    cout << "init list\n";
    while(1) {
        val++;
        cout << "val1 = " << val << "\n";
        if(callbackDoSthList[0]()) {
            // Execute the corresponding control flow action
            //return optCtlCall[2]();  // Execute the first control flow action (CONTINUTE)
            CONTINUE_M
        }
        cout << "val2 = " << val << "\n";
        if (val > 5){
            cout << endl;
            break;
        }
    }
    // Check if the callback returns true


    return false;
}

int main() {
    // Add control flow actions to optCtlCall
    optCtlCall.push_back(RETURN_TRUE);    // Action 1: return true
    optCtlCall.push_back(RETURN_FALSE);   // Action 2: return false
    optCtlCall.push_back(CONTINUE_ACTION); // Action 3: continue (simulated)

    // Add callback to callbackDoSthList
    callbackDoSthList.push_back(isCall);

    // Execute initialization and check if it succeeds
    if(init()) {
        cout << "init success\n";
    } else {
        cout << "init failed\n";
    }

    return 0;
}
