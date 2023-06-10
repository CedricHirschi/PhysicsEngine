#include "user.hpp"

#include "solver.hpp"

#define WINDOW_WIDTH 1000.0
#define WINDOW_HEIGHT 700.0

sf::Clock dt_clock;
float last_time = dt_clock.getElapsedTime().asSeconds();

struct User user;

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "SFML works!", sf::Style::Default);

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

        double dt = dt_clock.getElapsedTime().asSeconds() - last_time;
        last_time = dt_clock.getElapsedTime().asSeconds();

        window.clear({50, 50, 50, 255});

        user.OnUpdate(window, dt);

        window.display();
    }

    return 0;
}

// #include <SFML/Graphics.hpp>

// int main()
// {
//     sf::Vector2u window_size(800, 600);
//     sf::VideoMode video_mode(window_size);
//     auto window = sf::RenderWindow(video_mode, "SFML works!");
//     std::cout << "Created window" << std::endl;
//     window.setFramerateLimit(144);

//     while (window.isOpen())
//     {
//         for (auto event = sf::Event{}; window.pollEvent(event);)
//         {
//             if (event.type == sf::Event::Closed)
//             {
//                 window.close();
//             }
//         }

//         window.clear();
//         window.display();
//     }
// }