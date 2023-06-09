#include "solver.hpp"

void VerletSolver::apply_gravity()
{
    for (auto &object : m_objects)
    {
        object.accelerate(m_gravity);
    }
}

void VerletSolver::check_collisions(double dt)
{
    const double response_coef = 0.5f;
    const uint64_t objects_count = m_objects.size();

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < m_objects.size(); i++)
    {
        VerletObject &object_1 = m_objects[i];

        for (int j = i + 1; j < m_objects.size(); j++)
        {
            VerletObject &object_2 = m_objects[j];

            const sf::Vector2<double> v = object_1.position - object_2.position;
            double dist = v.lengthSq();
            const double min_dist = object_1.radius + object_2.radius;

            // Check overlapping
            if (dist < min_dist * min_dist)
            {
                dist = sqrt(dist);
                const sf::Vector2<double> n = v.normalized();
                const double mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
                const double mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
                const double delta = 0.5f * response_coef * (dist - min_dist);

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
    switch (m_constraint_type)
    {
    case 0:
        break;
    case 1:
        std::for_each(std::execution::par_unseq, m_objects.begin(), m_objects.end(), [&](VerletObject &object)
                      {
                  const auto v = m_constraint_center - object.position;
                  const double distance = v.lengthSq();
                  if (distance > (m_constraint_radius - object.radius) * (m_constraint_radius - object.radius))
                  {
                      const auto n = v / sqrt(distance);
                      object.position = m_constraint_center - n * (m_constraint_radius - object.radius);
                  } });
        break;
    case 2:
        std::for_each(std::execution::par_unseq, m_objects.begin(), m_objects.end(), [&](VerletObject &object)
                      {
                      if (object.position.x < m_constraint_center.x - m_constraint_radius + object.radius)
                      {
                          object.position.x = m_constraint_center.x - m_constraint_radius + object.radius;
                      }
                      else if (object.position.x > m_constraint_center.x + m_constraint_radius - object.radius)
                      {
                          object.position.x = m_constraint_center.x + m_constraint_radius - object.radius;
                      }

                      if (object.position.y < m_constraint_center.y - m_constraint_radius + object.radius)
                      {
                          object.position.y = m_constraint_center.y - m_constraint_radius + object.radius;
                      }
                      else if (object.position.y > m_constraint_center.y + m_constraint_radius - object.radius)
                      {
                          object.position.y = m_constraint_center.y + m_constraint_radius - object.radius;
                      } });
        break;
    default:
        break;
    }
}

void VerletSolver::update_objects(double dt)
{
    for (auto &object : m_objects)
    {
        object.position_update(dt);
    }
}

VerletObject &VerletSolver::add_object(sf::Vector2<double> position, double radius, sf::Color color)
{
    return m_objects.emplace_back(position, radius, color);
}

void VerletSolver::update(double _dt)
{
    m_frame_dt = _dt;
    m_time += m_frame_dt;
    double substep_dt = m_frame_dt / m_substeps;
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

void VerletSolver::set_constraints(uint8_t _type, sf::Vector2<double> _position, double _radius)
{
    m_constraint_type = _type;
    m_constraint_center = _position;
    m_constraint_radius = _radius;
}

void VerletSolver::set_gravity(sf::Vector2<double> _gravity)
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

const sf::Shape &VerletSolver::get_constraint()
{
    static sf::CircleShape circle_constraint;
    static sf::RectangleShape rect_constraint;

    switch (m_constraint_type)
    {
    case 0:
        break;
    case 1:
        circle_constraint.setRadius((float)m_constraint_radius);
        circle_constraint.setOrigin({(float)m_constraint_radius, (float)m_constraint_radius});
        circle_constraint.setPosition((sf::Vector2f)m_constraint_center);
        circle_constraint.setFillColor(sf::Color::Black);
        circle_constraint.setOutlineColor(sf::Color::White);
        circle_constraint.setOutlineThickness(2.0f);

        return circle_constraint;
    case 2:
        rect_constraint.setSize({(float)m_constraint_radius * 2.0f, (float)m_constraint_radius * 2.0f});
        rect_constraint.setOrigin({(float)m_constraint_radius, (float)m_constraint_radius});
        rect_constraint.setPosition((sf::Vector2f)m_constraint_center);
        rect_constraint.setFillColor(sf::Color::Black);
        rect_constraint.setOutlineColor(sf::Color::White);
        rect_constraint.setOutlineThickness(2.0f);

        return rect_constraint;
    }

    return rect_constraint;
}