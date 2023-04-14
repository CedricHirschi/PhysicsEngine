#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <memory>
#include <random>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "solver.hpp"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define LOG 1

struct VerletSolver solver;

sf::Clock dt_clock;
sf::Clock spawn_clock;
sf::Clock total_clock;

#if LOG
// file for writing number of objects and fps
std::ofstream file;
#endif

float randfloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int randint(int min, int max)
{
    return min + rand() % (max - min);
}

static sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * 3.14f);
    const float b = sin(t + 0.66f * 2.0f * 3.14f);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

std::vector<float> last_fps;

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "SFML works!", sf::Style::Default, settings);
    window.setFramerateLimit(240);

    solver.set_constraints({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, WINDOW_WIDTH / 4);
    solver.set_substeps(8);

    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
    }

    sf::Text fps(font, "FPS: uninitialized", 20);
    fps.setFillColor(sf::Color::White);
    fps.setPosition({10.0f, 10.0f});

    sf::Text numobj(font, "Objects: uninitialized", 20);
    numobj.setFillColor(sf::Color::White);
    numobj.setPosition({10.0f, 30.0f});

#if LOG
    // write header to file
    file.open("data.csv");
    file << "objects,fps" << std::endl;
    file.close();
#endif

    float last_time = dt_clock.getElapsedTime().asSeconds();
    bool spawn = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                solver.set_constraints({event.size.width / 2, event.size.height / 2}, event.size.width / 4);
                window.setView(sf::View(sf::FloatRect({0, 0}, sf::Vector2f(event.size.width, event.size.height))));
                solver.clear_objects();
            }

            // if r is pressed, reset the simulation
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                solver.clear_objects();
            }

            // if s is pressed, toggle spawning
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
            {
                spawn = !spawn;
            }

            // if escape is pressed, close the window
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }

            /* // string array of event types
            std::string event_types[] = {"Closed", "Resized", "LostFocus", "GainedFocus", "TextEntered", "KeyPressed", "KeyReleased", "MouseWheelMoved", "MouseWheelScrolled", "MouseButtonPressed", "MouseButtonReleased", "MouseMoved", "MouseEntered", "MouseLeft", "JoystickButtonPressed", "JoystickButtonReleased", "JoystickMoved", "JoystickConnected", "JoystickDisconnected", "TouchBegan", "TouchMoved", "TouchEnded", "SensorChanged", "Count"};

            std::cout << "Event: " << event_types[event.type] << std::endl; */
        }
        float dt = dt_clock.getElapsedTime().asSeconds() - last_time;
        last_time = dt_clock.getElapsedTime().asSeconds();

        solver.update(dt);

        // update fps
        last_fps.push_back(1.0f / dt);
        if (last_fps.size() > 100)
        {
            last_fps.erase(last_fps.begin());
        }

        // display fps to one decimal place
        fps.setString("FPS: " + std::to_string(int(std::accumulate(last_fps.begin(), last_fps.end(), 0.0f) / last_fps.size())));

        // if mouse is pressed, spawn a new object
        if (spawn_clock.getElapsedTime().asSeconds() > 0.1f && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            spawn_clock.restart();
            auto position = sf::Vector2f(sf::Mouse::getPosition(window));
            auto &object = solver.add_object(position, randfloat(3.0f, 20.0f));
            object.set_color(getRainbow(total_clock.getElapsedTime().asSeconds()));
        }

        // spawn new object every 0.5 seconds
        if (spawn_clock.getElapsedTime().asSeconds() > 0.05f && spawn)
        {
            spawn_clock.restart();
            auto position = solver.get_constraint().getPosition() - sf::Vector2f(0.0f, solver.get_constraint().getRadius() / 2.0f);
            auto angle = 3.14f / 2.0f * sin(total_clock.getElapsedTime().asSeconds());
            auto velocity = sf::Vector2f(100.0f * sin(angle), 100.0f * cos(angle));
            auto &object = solver.add_object(position, randfloat(3.0f, 20.0f));
            object.set_velocity(velocity, dt);
            object.set_color(getRainbow(total_clock.getElapsedTime().asSeconds()));

#if LOG
            // write number of objects and fps to file
            file.open("data.csv", std::ios_base::app);
            file << solver.get_objects().size() << "," << int(std::accumulate(last_fps.begin(), last_fps.end(), 0.0f) / last_fps.size()) << std::endl;
            file.close();
#endif
        }

        numobj.setString("Objects: " + std::to_string(solver.get_objects().size()));

        window.clear({50, 50, 50, 255});
        window.draw(fps);
        window.draw(numobj);
        window.draw(solver.get_constraint());
        for (auto &object : solver.get_objects())
        {
            sf::CircleShape shape(object.radius);
            shape.setFillColor(object.color);
            shape.setOrigin({object.radius, object.radius});
            shape.setPosition(object.position);
            window.draw(shape);
        }
        window.display();
    }

    return 0;
}
