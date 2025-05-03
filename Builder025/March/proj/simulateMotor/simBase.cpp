#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <thread>
#include <chrono>

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

    // Create some shapes for display
    sf::RectangleShape motorShape(sf::Vector2f(100, 100));
    motorShape.setFillColor(sf::Color::Green);
    motorShape.setPosition(350, 250);

    // Create text to display motor status
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
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

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Handle user input (start, stop, speed control)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {  // 'S' to start
            motor.start();
            motorShape.setFillColor(sf::Color::Green); // Green means running
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {  // 'T' to stop
            motor.stop();
            motorShape.setFillColor(sf::Color::Red);  // Red means stopped
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {  // Up arrow to increase speed
            motor.setSpeed(motor.getSpeed() + 10);
            if (motor.getSpeed() > 100) motor.setSpeed(100);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {  // Down arrow to decrease speed
            motor.setSpeed(motor.getSpeed() - 10);
            if (motor.getSpeed() < 0) motor.setSpeed(0);
        }

        // Update status text
        if (motor.getIsRunning()) {
            statusMessage = "Motor is running.\nSpeed: " + std::to_string(motor.getSpeed()) + "%";
        } else {
            statusMessage = "Motor is stopped.\nSpeed: " + std::to_string(motor.getSpeed()) + "%";
        }
        statusText.setString(statusMessage);

        // Clear the screen
        window.clear();
        
        // Draw motor and status text
        window.draw(motorShape);
        window.draw(statusText);

        // Display everything we just drew
        window.display();

        // Adding a small delay for responsiveness
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

