#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

#define SINGLE_EXPR(expression) [](const float& t) ->float {return expression;};

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
constexpr auto PI = std::numbers::pi_v<float>;
constexpr float hPI = PI / 2;

constexpr float c1 = 1.70158;
constexpr float c2 = c1 * 1.525;


// global tween function

using transitionFuncType = std::function<float(float)>;

transitionFuncType transition = [](const float& t) ->float {
    return t;
};

std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - transition(t)) * a + (transition(t)) * b;
};


inline transitionFuncType getTransition(const char& c, const transitionFuncType& og) {

    switch(c) {

        case '1' : return SINGLE_EXPR(t); // linear
        case '2' : return SINGLE_EXPR(1 - std::pow((1-t),4)); // easeOutQuart
        case '3' : return SINGLE_EXPR(1 - std::cos(t * hPI)); // easeInSine
        case '4' : return SINGLE_EXPR(t == 1 ? 1 : 1 - std::pow(2, -10 * t)); //  easeOutExpo
        case '5' : return SINGLE_EXPR(std::sqrt(1 - std::pow(t - 1, 2));); // easeOutCirc
        case '6' : return SINGLE_EXPR(t < 0.5 ? (1 - std::sqrt(1 - std::pow(2 * t, 2))) / 2: (std::sqrt(1 - std::pow(-2 * t + 2, 2)) + 1) / 2); // easeInOutCirc
        case '7' : return SINGLE_EXPR(1 - std::sqrt(1 - std::pow(t, 2))); // easeInCirc
        case '8' : return SINGLE_EXPR(t == 0 ? 0 : std::pow(2, 10 * t - 10)); // easeInExpo
        case '9' : return SINGLE_EXPR(t < 0.5 ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2); // easeInOutQuad
        default: return og;
}

}




void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        } else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>()) {
            transition = getTransition(static_cast<char>(keyPressed->unicode), transition);
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    constexpr float FrameStep = 0.05;
    constexpr float AnimationTime = 1 / FrameStep; // # of frames it takes for t to go from 0 to 1.
    static int t = 0;
    static int sign = 1;

    // t goes up and down by 0.2
    // which means its takes 5 frames to go from one side of the screen to the other.
    // so t*0.2

    constexpr float radius = 0.05f * WINDOW_WIDTH;
    constexpr float diameter = 2*radius ; 

    sf::CircleShape circle(radius);

    circle.setPosition(sf::Vector2f(tween(0, WINDOW_WIDTH - diameter, (t) * FrameStep), WINDOW_HEIGHT / 3));

    t += sign;
    if (t <= 0) sign = 1;
    else if (t >= AnimationTime) sign = -1; // making sure t oscilates between 0 and AnimationTime


    window.draw(circle);


    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
