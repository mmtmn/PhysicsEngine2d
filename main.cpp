#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

class Vector2D {
public:
    double x, y;

    Vector2D(double x = 0.0, double y = 0.0) : x(x), y(y) {}

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
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

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

        // Boundary checking
        float radius = 30.0f; // Circle radius
        if (v1.x < radius) v1.x = radius;
        if (v1.y < radius) v1.y = radius;
        if (v1.x > 800 - radius) v1.x = 800 - radius; // Window width
        if (v1.y > 600 - radius) v1.y = 600 - radius; // Window height

        // Clear screen
        window.clear();

        // Update positions
        circle1.setPosition(v1.x, v1.y);
        circle2.setPosition(v2.x, v2.y);

        // Check collision
        if (checkCollision(v1, v2, 30, 30)) {
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
