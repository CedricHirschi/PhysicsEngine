#include "user.hpp"

#include "solver.hpp"

#define WINDOW_WIDTH 1600.0f
#define WINDOW_HEIGHT 900.0f

sf::Clock dt_clock;
float last_time = dt_clock.getElapsedTime().asSeconds();

struct User user;

int main()
{
    sf::ContextSettings settings;
    // settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "SFML works!", sf::Style::Default, settings);
    // window.setFramerateLimit(240);

    user.OnStartup(window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                break;
            }
            user.OnEvent(window, event);
        }

        float dt = dt_clock.getElapsedTime().asSeconds() - last_time;
        last_time = dt_clock.getElapsedTime().asSeconds();

        window.clear({50, 50, 50, 255});

        user.OnUpdate(window, dt);

        window.display();
    }

    return 0;
}
