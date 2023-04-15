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
    sf::Vector2f m_gravity = {0.0f, 2000.0f};
    std::vector<VerletObject> m_objects;
    float m_time = 0.0f;
    float m_frame_dt = 0.0f;
    sf::Vector2f m_constraint_center = {400.0f, 300.0f};
    float m_constraint_radius = 200.0f;

    void apply_gravity();

    void check_collisions(float dt);

    void apply_constraints();

    void update_objects(float dt);

public:
    VerletSolver() = default;

    VerletObject &add_object(sf::Vector2f position, float radius);

    void update(float _dt);

    void set_substeps(unsigned _substeps);
    void set_constraints(sf::Vector2f _postition, float _radius);
    void set_gravity(sf::Vector2f _gravity);

    void clear_objects();

    const std::vector<VerletObject> &get_objects();
    const sf::CircleShape &get_constraint();
};

#endif // SOLVER_HPP