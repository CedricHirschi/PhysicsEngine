#include "solver.hpp"

void VerletSolver::apply_gravity()
{
    for (auto &object : m_objects)
    {
        object.accelerate(m_gravity);
    }
}

void VerletSolver::check_collisions(float dt)
{
    const float response_coef = 0.5f;
    const uint64_t objects_count = m_objects.size();

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < m_objects.size(); i++)
    {
        VerletObject &object_1 = m_objects[i];

        for (int j = i + 1; j < m_objects.size(); j++)
        {
            VerletObject &object_2 = m_objects[j];

            const sf::Vector2f v = object_1.position - object_2.position;
            float dist = v.lengthSq();
            const float min_dist = object_1.radius + object_2.radius;

            // Check overlapping
            if (dist < min_dist * min_dist)
            {
                dist = sqrt(dist);
                const sf::Vector2f n = v.normalized();
                const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
                const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
                const float delta = 0.5f * response_coef * (dist - min_dist);

// Update positions
#pragma omp critical
                {
                    object_1.position -= n * (mass_ratio_2 * delta);
                    object_2.position += n * (mass_ratio_1 * delta);
                }
            }
        }
    }
}

void VerletSolver::apply_constraints()
{
    std::for_each(std::execution::par_unseq, m_objects.begin(), m_objects.end(), [&](VerletObject &object)
                  {
                      const auto v = m_constraint_center - object.position;
                      const float distance = v.lengthSq();
                      if (distance > (m_constraint_radius - object.radius) * (m_constraint_radius - object.radius))
                      {
                          const auto n = v / sqrtf(distance);
                          object.position = m_constraint_center - n * (m_constraint_radius - object.radius);
                      } });
}

void VerletSolver::update_objects(float dt)
{
    for (auto &object : m_objects)
    {
        object.position_update(dt);
    }
}

VerletObject &VerletSolver::add_object(sf::Vector2f position, float radius)
{
    return m_objects.emplace_back(position, radius);
}

void VerletSolver::update(float _dt)
{
    m_frame_dt = _dt;
    m_time += m_frame_dt;
    float substep_dt = m_frame_dt / m_substeps;
    for (unsigned i = 0; i < m_substeps; ++i)
    {
        apply_gravity();
        check_collisions(substep_dt);
        apply_constraints();
        update_objects(substep_dt);
    }
}

void VerletSolver::set_substeps(unsigned _substeps)
{
    m_substeps = _substeps;
}

void VerletSolver::set_constraints(sf::Vector2f _position, float _radius)
{
    m_constraint_center = _position;
    m_constraint_radius = _radius;
}

void VerletSolver::set_gravity(sf::Vector2f _gravity)
{
    m_gravity = _gravity;
}

void VerletSolver::clear_objects()
{
    m_objects.clear();
}

const std::vector<VerletObject> &VerletSolver::get_objects()
{
    return m_objects;
}

const sf::CircleShape &VerletSolver::get_constraint()
{
    static sf::CircleShape constraint;
    constraint.setRadius(m_constraint_radius);
    constraint.setOrigin({m_constraint_radius, m_constraint_radius});
    constraint.setPosition(m_constraint_center);
    constraint.setFillColor(sf::Color::Black);
    constraint.setOutlineColor(sf::Color::White);
    constraint.setOutlineThickness(2.0f);
    return constraint;
}