#ifndef USER_HPP
#define USER_HPP

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

struct User
{
    void OnStartup(sf::RenderWindow &window);
    void OnUpdate(sf::RenderWindow &window, double dt);
    void OnEvent(sf::RenderWindow &window, sf::Event &event);
};

#endif // USER_HPP