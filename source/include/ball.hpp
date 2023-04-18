#ifndef BALL_HPP
#define BALL_HPP

#include <iostream>

#include <SFML/Graphics.hpp>

struct VerletObject
{
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    float radius = 10.0f;
    sf::Color color = sf::Color::White;

    VerletObject() = default;
    VerletObject(sf::Vector2f _position, float _radius)
        : position(_position), position_last(_position), radius(_radius)
    {
    }

    void position_update(float dt);

    void accelerate(sf::Vector2f _acceleration);

    void set_velocity(sf::Vector2f _velocity, float dt);

    void set_color(sf::Color _color);

    void add_velocity(sf::Vector2f _velocity, float dt);

    sf::Vector2f get_velocity(float dt);
};

#endif // BALL_HPP