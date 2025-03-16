#include <climits>
#include <iostream>
#include <stdexcept>  // for std::overflow_error, std::underflow_error
using namespace std;

class Number {
public:
    int value;

    // Constructor
    Number(int v) : value(v) {}

    // Các phép toán bạn yêu cầu
    friend Number operator+ (const Number& x, const Number& y);
    friend Number operator- (const Number& x, const Number& y);
    friend Number operator* (const Number& x, const Number& y);
    friend Number operator/ (const Number& x, const Number& y);

    // Các lớp ngoại lệ
    class Overflow {};
    class Underflow {};
    class DivideByZero {};
};

// Định nghĩa các phép toán cho lớp Number

Number operator+ (const Number& x, const Number& y) {
    if (x.value > INT_MAX - y.value) throw Number::Overflow();
    return Number(x.value + y.value);
}

Number operator- (const Number& x, const Number& y) {
    if (x.value < INT_MIN + y.value) throw Number::Underflow();
    return Number(x.value - y.value);
}

Number operator* (const Number& x, const Number& y) {
    if (x.value > INT_MAX / y.value) throw Number::Overflow();
    return Number(x.value * y.value);
}

Number operator/ (const Number& x, const Number& y) {
    if (y.value == 0) throw Number::DivideByZero();
    return Number(x.value / y.value);
}

// Hàm xử lý phép toán và ngoại lệ
void f(Number x, Number y) {
    try {
        Number sum  = x + y;
        cout << "Sum: " << sum.value << endl;

        Number diff = x - y;
        cout << "Difference: " << diff.value << endl;

        Number prod = x * y;
        cout << "Product: " << prod.value << endl;

        Number quot = x / y;
        cout << "Quotient: " << quot.value << endl;
    }
    catch (Number::Overflow& exception) {
        cout << "Overflow error!" << endl;
    }
    catch (Number::Underflow& exception) {
        cout << "Underflow error!" << endl;
    }
    catch (Number::DivideByZero& exception) {
        cout << "Divide by zero error!" << endl;
    }
}

int main() {
    Number num1(1000);
    Number num2(2000);

    // Gọi hàm với các số đã tạo
    f(num1, num2);

    // Thử với số chia cho 0
    Number num3(10);
    Number num4(0);
    f(num3, num4); // sẽ gặp lỗi chia cho 0

    return 0;
}
