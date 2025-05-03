#include <iostream>
#include <cmath>  // To use isnan() and isinf()
#include <stdexcept>  // To handle exceptions

using namespace std;

int main() {
    double num1, num2, result;

    // Prompt the user to input the first number
    cout << "Enter the first number: ";
    cin >> num1;

    // Prompt the user to input the second number
    cout << "Enter the second number: ";
    cin >> num2;

    try {
        // Perform the division
        result = num1 / num2;

        // Check if the result is NaN (Not a Number)
        if (isnan(result)) {
            throw domain_error("Error: The result is NaN (Not a Number).");
        }

        // Check if the result is infinity (positive or negative)
        if (isinf(result)) {
            if (result > 0) {
                throw domain_error("Error: The result is infinity (Infinity).");
            } else if (result < 0) {
                throw domain_error("Error: The result is negative infinity (-Infinity).");
            }
        }

        // Display the valid result
        cout << "The result is: " << result << endl;
    }
    catch (const domain_error& e) {
        // Catch and display the exception message
        cout << e.what() << endl;
    }

    return 0;
}

