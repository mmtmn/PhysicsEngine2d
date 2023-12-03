#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

class Vector2D {
public:
    double x, y;
    double vx, vy; // Velocity components

    Vector2D(double x = 0.0, double y = 0.0, double vx = 0.0, double vy = 0.0)
        : x(x), y(y), vx(vx), vy(vy) {}

    // Addition
    Vector2D operator+(const Vector2D& v) const {
        return {x + v.x, y + v.y};
    }

    // Subtraction
    Vector2D operator-(const Vector2D& v) const {
        return {x - v.x, y - v.y};
    }

    // Multiplication by scalar
    Vector2D operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }

    // Dot product
    double dot(const Vector2D& v) const {
        return x * v.x + y * v.y;
    }

    // Magnitude
    double magnitude() const {
        return sqrt(x * x + y * y);
    }

    // Normalization
    Vector2D& normalize() {
        double mag = magnitude();
        if (mag == 0) throw std::runtime_error("Cannot normalize zero vector");
        x /= mag;
        y /= mag;
        return *this;
    }

    // Distance
    double distance(const Vector2D& v) const {
        return (*this - v).magnitude();
    }

    // Angle in radians
    double angle() const {
        return atan2(y, x);
    }

    // Division by scalar
    Vector2D operator/(double scalar) const {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        return {x / scalar, y / scalar};
    }

    // Equality check
    bool operator==(const Vector2D& v) const {
        return x == v.x && y == v.y;
    }

    // Update method for motion under gravity
    void update(double deltaTime, const Vector2D& acceleration) {
        vx += acceleration.x * deltaTime;
        vy += acceleration.y * deltaTime;
        x += vx * deltaTime + 0.5 * acceleration.x * deltaTime * deltaTime;
        y += vy * deltaTime + 0.5 * acceleration.y * deltaTime * deltaTime;
    }

    // Method for bouncing on collision
    void bounce(double elasticity = 0.7) {
        vy = -vy * elasticity;
    }

    // Inequality check
    bool operator!=(const Vector2D& v) const {
        return x != v.x || y != v.y;
    }

    // Output stream
    friend ostream& operator<<(ostream& os, const Vector2D& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};

// Collision detection (basic)
bool checkCollision(const Vector2D& v1, const Vector2D& v2, double radius1, double radius2) {
    double distance = v1.distance(v2);
    return distance < (radius1 + radius2);
}

int main() {
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Physics Engine with Graphics");

    // Define border dimensions
    const float borderThickness = 10.0f;
    const sf::Vector2f borderPosition(20, 20);
    const sf::Vector2f borderSize(760, 560);
    const Vector2D gravity(0, 980); // Adjust as needed

    // Create border
    sf::RectangleShape border(borderSize);
    border.setPosition(borderPosition);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(borderThickness);
    border.setOutlineColor(sf::Color::White);

    // Create two circles representing our vectors
    sf::CircleShape circle1(30); // Radius 30
    circle1.setFillColor(sf::Color::Red);
    Vector2D v1(100.0, 100.0);

    sf::CircleShape circle2(30); // Radius 30
    circle2.setFillColor(sf::Color::Blue);
    Vector2D v2(300.0, 300.0);

    // Initialize clock for delta time
    sf::Clock clock;

    while (window.isOpen()) {
        // Calculate delta time
        sf::Time deltaTime = clock.restart();

        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Apply gravity and update positions
        v1.update(deltaTime.asSeconds(), gravity);
        v2.update(deltaTime.asSeconds(), gravity);



        // Enhanced user input handling with delta time
        float moveSpeed = 100.0f; // Speed in pixels per second
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            v1.x -= moveSpeed * deltaTime.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            v1.x += moveSpeed * deltaTime.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            v1.y -= moveSpeed * deltaTime.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            v1.y += moveSpeed * deltaTime.asSeconds();
        }

        // Boundary checking with border adjustment
        float radius = 30.0f; // Circle radius

        float minX = borderPosition.x + radius + borderThickness;
        float maxX = borderPosition.x + borderSize.x - radius - borderThickness;
        float minY = borderPosition.y + radius + borderThickness;
        float maxY = borderPosition.y + borderSize.y - radius - borderThickness;

        if (v1.x - radius < minX) v1.x = minX + radius;
        if (v1.y - radius < minY) v1.y = minY + radius;
        if (v1.x + radius > maxX) v1.x = maxX - radius;
        if (v1.y + radius > maxY) v1.y = maxY - radius;

        if (checkCollision(v1, v2, radius, radius)) {
            // Apply bounce
            v1.bounce();
            v2.bounce();
        }

        // Clear screen
        window.clear();

        window.draw(border);

        // Update positions
        circle1.setPosition(v1.x - radius, v1.y - radius); // Adjusting for the circle's top-left corner position
        circle2.setPosition(v2.x - radius, v2.y - radius); // Adjusting for the circle's top-left corner position

        // Check collision
        if (checkCollision(v1, v2, radius, radius)) {
            circle1.setFillColor(sf::Color::Green);
            circle2.setFillColor(sf::Color::Green);
        } else {
            circle1.setFillColor(sf::Color::Red);
            circle2.setFillColor(sf::Color::Blue);
        }

        // Draw the circles
        window.draw(circle1);
        window.draw(circle2);

        // Update the window
        window.display();
    }

    return 0;
}
