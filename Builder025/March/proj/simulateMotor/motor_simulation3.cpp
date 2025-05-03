#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

class Motor {
private:
    bool isRunning;  // Motor status (running or stopped)
    int speed;       // Motor speed (from 0 to 100)

public:
    Motor() : isRunning(false), speed(0) {}

    void start() {
        isRunning = true;
    }

    void stop() {
        isRunning = false;
    }

    void setSpeed(int newSpeed) {
        if (newSpeed >= 0 && newSpeed <= 100) {
            speed = newSpeed;
        }
    }

    bool getIsRunning() const {
        return isRunning;
    }

    int getSpeed() const {
        return speed;
    }
};

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Motor Simulation");

    Motor motor;  // Create motor object

    // Create a circle to represent the motor body
    sf::CircleShape motorCircle(50);  // Circle with radius 50
    motorCircle.setFillColor(sf::Color::Green);
    motorCircle.setPosition(350, 250);

    // Create a line inside the motor to represent the rotating shaft
    sf::RectangleShape motorShaft(sf::Vector2f(100, 5));  // A rectangle as the shaft
    motorShaft.setFillColor(sf::Color::Black);
    motorShaft.setOrigin(0, 2);  // Set the origin to the left side (center of rotation)
    motorShaft.setPosition(400, 300);  // Set the center of the circle

    // Create text to display motor status
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cout << "Error loading font" << std::endl;
        return -1;
    }

    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(10, 10);

    std::string statusMessage = "Motor is stopped.\nSpeed: 0%";
    statusText.setString(statusMessage);

    float rotationAngle = 0.0f;  // Rotation angle for the motor shaft
    // Use sf::Clock to manage the 100ms interval
    sf::Clock sfmlClock; // sf::Clock to manage the 100ms interval

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Check if 100ms has passed
        if (sfmlClock.getElapsedTime().asMilliseconds() >= 100) {
            sfmlClock.restart();  // Reset the clock


            //            // Check if 100ms has passed
            //            if (clock.getElapsedTime().asMilliseconds() >= 100) {
            //                clock.restart();  // Reset the clock

            // Handle user input (start, stop, speed control)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {  // 'S' to start
                motor.start();
                motorCircle.setFillColor(sf::Color::Green); // Green means running
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {  // 'T' to stop
                motor.stop();
                motorCircle.setFillColor(sf::Color::Red);  // Red means stopped
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {  // Up arrow to increase speed
                motor.setSpeed(motor.getSpeed() + 2);
                if (motor.getSpeed() > 100) motor.setSpeed(100);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {  // Down arrow to decrease speed
                motor.setSpeed(motor.getSpeed() - 2);
                if (motor.getSpeed() < 0) motor.setSpeed(0);
            }
        }

        // Update status text
        if (motor.getIsRunning()) {
            statusMessage = "Motor is running.\nSpeed: " + std::to_string(motor.getSpeed()) + "%";
        } else {
            statusMessage = "Motor is stopped.\nSpeed: " + std::to_string(motor.getSpeed()) + "%";
        }
        statusText.setString(statusMessage);

        // Update the motor shaft rotation based on the speed
        if (motor.getIsRunning()) {
            rotationAngle += (motor.getSpeed() * motor.getSpeed() * 0.023f);  // Rotation speed is proportional to the motor speed
            if (rotationAngle >= 360) {
                rotationAngle = 0;  // Reset the angle to prevent overflow
            }
            motorShaft.setRotation(rotationAngle);  // Apply the rotation
        }

        // Clear the screen
        window.clear();

        // Draw the motor and shaft
        window.draw(motorCircle);
        window.draw(motorShaft);

        // Draw the status text
        window.draw(statusText);

        // Display everything we just drew
        window.display();

        // Adding a small delay for responsiveness
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return 0;
}

