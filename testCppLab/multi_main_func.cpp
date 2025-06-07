#include <iostream>

namespace A {
    int main( int argc, char *argv[] ) {
        std::cout << "Namespace A main\n";
        return 0; 
    }
    void mainA() {
        std::cout << "Namespace A mainA\n";
    }
}

namespace B {
    void main() {
        std::cout << "Namespace B main\n";
    }
}
using namespace A;
int main() {
    //A::main();
    const char* args[] = {"program", "arg1", "arg2"};
    A::main(3, const_cast<char**>(args)); // chuyển const char** sang char** để gọi
    mainA(); // khong loi
    B::main();
    return 0;
}
