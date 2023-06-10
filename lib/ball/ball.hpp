#ifndef BALL_HPP
#define BALL_HPP

#include <iostream>

#include <SFML/Graphics.hpp>

struct VerletObject
{
    sf::Vector2<double> position;
    sf::Vector2<double> position_last;
    sf::Vector2<double> acceleration;
    double radius = 10.0f;
    sf::Color color = sf::Color::White;

    VerletObject() = default;
    VerletObject(sf::Vector2<double> _position, double _radius, sf::Color _color = sf::Color::White)
        : position(_position), position_last(_position), radius(_radius), color(_color)
    {
    }

    void position_update(double dt);

    void accelerate(sf::Vector2<double> _acceleration);

    void set_velocity(sf::Vector2<double> _velocity, double dt);

    void set_color(sf::Color _color);

    void add_velocity(sf::Vector2<double> _velocity, double dt);

    sf::Vector2<double> get_velocity(double dt);
};

#endif // BALL_HPP