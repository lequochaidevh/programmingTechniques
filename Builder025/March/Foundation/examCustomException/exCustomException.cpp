#include <iostream>
#include <exception>
#include <string>

// Custom exception class with parameters passed
class CustomException : public std::exception {
private:
    std::string message;  // Error message
    int errorCode;        // Error code

public:
    // Constructor that accepts the error message and error code
    CustomException(const std::string& msg, int code) : message(msg), errorCode(code) {}

    // Override the what() function to return the error message
    const char* what() const noexcept override {
        return message.c_str();
    }

    // Getter function to return the error code
    int getErrorCode() const {
        return errorCode;
    }
};

// Function that may throw an exception
void testFunction() {
    // Simulate an error condition and throw an exception with a message and error code
    bool errorOccurred = true;
    if (errorOccurred) {
        throw CustomException("An error occurred in testFunction!", 101);  // Throwing the custom exception
    }
}

int main() {
    try {
        testFunction(); // Calling the function that may throw an exception
    } catch (const CustomException& e) {
        // Catch and handle the exception
        std::cout << "Exception caught: " << e.what() << std::endl;  // Print the error message
        std::cout << "Error code: " << e.getErrorCode() << std::endl;  // Print the error code
    }

    return 0;
}
