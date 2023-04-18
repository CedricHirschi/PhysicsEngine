#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include <execution>
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "ball.hpp"

class VerletSolver
{
    unsigned m_substeps = 1;
    sf::Vector2<double> m_gravity = {0.0f, 2000.0f};

    std::vector<VerletObject> m_objects;

    double m_time = 0.0f;
    double m_frame_dt = 0.0f;

    uint8_t m_constraint_type = 2; // 0 - none, 1 - circle, 2 - rectangle
    sf::Vector2<double> m_constraint_center = {400.0f, 300.0f};
    double m_constraint_radius = 200.0f;

    void apply_gravity();

    void check_collisions(double dt);

    void apply_constraints();

    void update_objects(double dt);

public:
    VerletSolver() = default;

    VerletObject &add_object(sf::Vector2<double> position, double radius);

    void update(double _dt);

    void set_substeps(unsigned _substeps);
    void set_constraints(uint8_t _type, sf::Vector2<double> _position, double _radius);
    void set_gravity(sf::Vector2<double> _gravity);

    void clear_objects();

    const std::vector<VerletObject> &get_objects();
    const sf::Shape &get_constraint();
};

#endif // SOLVER_HPP