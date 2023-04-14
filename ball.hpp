#pragma once

#define FRICTION 0

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

    void position_update(float dt)
    {
        // calculate position difference
        sf::Vector2f position_diff = (position - position_last);
        // apply friction
#if FRICTION
        position_diff *= 0.999f;
#endif
        // update last position
        position_last = position;
        // update position
        position = position + position_diff + acceleration * (dt * dt);
        // reset acceleration
        acceleration = {};
    }

    void accelerate(sf::Vector2f _acceleration)
    {
        acceleration += _acceleration;
    }

    void set_velocity(sf::Vector2f _velocity, float dt)
    {
        position_last = position - (_velocity * dt);
    }

    void set_color(sf::Color _color)
    {
        color = _color;
    }

    void add_velocity(sf::Vector2f _velocity, float dt)
    {
        position_last -= _velocity * dt;
    }

    sf::Vector2f get_velocity(float dt)
    {
        return (position - position_last) / dt;
    }
};