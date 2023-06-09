#include "ball.hpp"

#define FRICTION 0

void VerletObject::position_update(double dt)
{
    // calculate position difference
    sf::Vector2<double> position_diff = (position - position_last);
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

void VerletObject::accelerate(sf::Vector2<double> _acceleration)
{
    acceleration += _acceleration;
}

void VerletObject::set_velocity(sf::Vector2<double> _velocity, double dt)
{
    position_last = position - (_velocity * dt);
}

void VerletObject::add_velocity(sf::Vector2<double> _velocity, double dt)
{
    position_last -= _velocity * dt;
}

sf::Vector2<double> VerletObject::get_velocity(double dt)
{
    return (position - position_last) / dt;
}

void VerletObject::set_color(sf::Color _color)
{
    color = _color;
}