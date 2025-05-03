#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

class ShapeMotor {
private:
    sf::Shape* motorOutside;     // Pointer to the shape representing the motor's outer part
    sf::Shape* motorInside;      // Pointer to the shape representing the motor's inner shaft

public:
    // Constructor accepting pointers to motor shapes
    ShapeMotor(sf::Shape* outside, sf::Shape* inside)
        : motorOutside(outside), motorInside(inside) {}

    // Destructor to delete motor shapes if they were dynamically allocated (optional)
    ~ShapeMotor() {
        // Only delete if you allocated them dynamically in the outside code
        delete motorOutside;   // Uncomment if dynamically allocated
        delete motorInside;    // Uncomment if dynamically allocated
    }

    // Methods to access the motor parts
    sf::Shape* getMotorOutside() {
        return motorOutside;
    }

    sf::Shape* getMotorInside() {
        return motorInside;
    }

    // Method to update the motor's rotation (if needed)
    void updateMotor(float angle) {
        motorInside->setRotation(angle);  // Rotate the inner part (shaft)
    }
};

class Motor {
private:
    bool isRunning;              // Motor status (running or stopped)
    int speed;                   // Motor speed (from 0 to 100)
    sf::Shape* motorOutside;     // Pointer to the shape representing the motor's outer part
    sf::Shape* motorInside;      // Pointer to the shape representing the motor's inner shaft
    float rotationAngle;         // Rotation angle for the motor's shaft

public:
    // Constructor now accepts parameters for size, position, and types of shapes
    Motor(float motorRadius, float motorWidth, float motorHeight, float motorPositionX, float motorPositionY) 
        : isRunning(false), speed(0), rotationAngle(0.0f) {

        // Create the motor outside shape (circle)
        motorOutside = new sf::CircleShape(motorRadius);   // Outer radius of the motor
        motorOutside->setFillColor(sf::Color::Green); // Green color when running
        motorOutside->setPosition(motorPositionX, motorPositionY); // Position of the motor
        //motorOutside->setPosition(motorPositionX - motorRadius, motorPositionY - motorRadius); // Position of the motor (centered)

        // Create the motor inside shape (shaft)
        motorInside = new sf::RectangleShape(sf::Vector2f(motorWidth, motorHeight));  // Shaft size (width, height)
        motorInside->setFillColor(sf::Color::Black);  // Black color for the shaft
        motorInside->setOrigin(-1, motorHeight / 2); // Origin at the left side of the shaft (centered vertically)
        //motorInside->setPosition(motorPositionX + motorRadius, motorPositionY); // Position at the center of the circle
        motorInside->setPosition(motorPositionX+motorRadius, motorPositionY+motorRadius); // Position at the center of the circle
    }

    // Destructor to delete dynamically allocated shapes
    ~Motor() {
        delete motorOutside;
        delete motorInside;
    }

    // Methods to start and stop the motor
    void start() {
        isRunning = true;
        motorOutside->setFillColor(sf::Color::Green); // Green means running
    }

    void stop() {
        isRunning = false;
        motorOutside->setFillColor(sf::Color::Red);  // Red means stopped
    }

    // Method to set the speed of the motor
    void setSpeed(int newSpeed) {
        if (newSpeed >= 0 && newSpeed <= 100) {
            speed = newSpeed;
        }
    }

    // Method to get the motor's running status
    bool getIsRunning() const {
        return isRunning;
    }

    // Method to get the current speed of the motor
    int getSpeed() const {
        return speed;
    }

    // Method to update the motor's behavior (shaft rotation)
    void on_update() {
        if (isRunning) {
            rotationAngle += (speed * speed * 0.023f);  // Proportional rotation speed based on motor speed
            if (rotationAngle >= 360) {
                rotationAngle = 0;  // Reset the angle to prevent overflow
            }
            motorInside->setRotation(rotationAngle);  // Apply the rotation to the shaft
        }
    }

    // Methods to access the shapes
    sf::Shape* getMotorOutside() {
        return motorOutside;
    }

    sf::Shape* getMotorInside() {
        return motorInside;
    }

    // Method to update the motor's status text
    std::string getStatusMessage() const {
        if (isRunning) {
            return "Motor is running.\nSpeed: " + std::to_string(speed) + "%";
        } else {
            return "Motor is stopped.\nSpeed: " + std::to_string(speed) + "%";
        }
    }
};

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Motor Simulation");

    // Create motor object with specific parameters for radius, width, height, and position
    Motor motor(50, 50-2, 5, 350, 250);  // Example: motor with 50 radius, 100 width (shaft), 5 height (shaft), and positioned at (350, 250)

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

    // Clock to manage the 100ms interval
    sf::Clock sfmlClock;

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

            // Handle user input (start, stop, speed control)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {  // 'S' to start
                motor.start();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {  // 'T' to stop
                motor.stop();
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

        // Update the motor behavior
        motor.on_update();

        // Update the status text
        statusText.setString(motor.getStatusMessage());

        // Clear the screen
        window.clear();
        
        // Draw the motor (outside and inside shapes)
        window.draw(*motor.getMotorOutside());
        window.draw(*motor.getMotorInside());

        // Draw the status text
        window.draw(statusText);

        // Display everything we just drew
        window.display();

        // Adding a small delay for responsiveness
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return 0;
}

