#pragma once

#include <vector>
#include <execution>

#include <SFML/Graphics.hpp>

#include "ball.hpp"

class VerletSolver
{
    unsigned m_substeps = 1;
    sf::Vector2f m_gravity = {0.0f, 1000.0f};
    std::vector<VerletObject> m_objects;
    float m_time = 0.0f;
    float m_frame_dt = 0.0f;
    sf::Vector2f m_constraint_center = {400.0f, 300.0f};
    float m_constraint_radius = 200.0f;

    void apply_gravity()
    {
        for (auto &object : m_objects)
        {
            object.accelerate(m_gravity);
        }
    }

    void check_collisions(float dt)
    {
        const float response_coef = 0.5f;
        const uint64_t objects_count = m_objects.size();
        // Iterate on all objects
        // for (uint64_t i{0}; i < objects_count; ++i)
        // {
        //     VerletObject &object_1 = m_objects[i];
        //     // Iterate on object involved in new collision pairs
        //     for (uint64_t k{i + 1}; k < objects_count; ++k)
        //     {
        //         VerletObject &object_2 = m_objects[k];
        //         const sf::Vector2f v = object_1.position - object_2.position;
        //         const float dist2 = v.x * v.x + v.y * v.y;
        //         const float min_dist = object_1.radius + object_2.radius;
        //         // Check overlapping
        //         if (dist2 < min_dist * min_dist)
        //         {
        //             const float dist = sqrt(dist2);
        //             const sf::Vector2f n = v / dist;
        //             const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
        //             const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
        //             const float delta = 0.5f * response_coef * (dist - min_dist);
        //             // Update positions
        //             object_1.position -= n * (mass_ratio_2 * delta);
        //             object_2.position += n * (mass_ratio_1 * delta);
        //         }
        //     }
        // }

        // rewritten with std::for_each and std::execution::par
        // std::for_each(std::execution::par_unseq, m_objects.begin(), m_objects.end(), [&](VerletObject &object_1)
        //               { std::for_each(std::execution::par_unseq, m_objects.begin(), m_objects.end(), [&](VerletObject &object_2)
        //                               {

        //         if(&object_1 == &object_2) return;

        //         const sf::Vector2f v = object_1.position - object_2.position;
        //         float dist = v.lengthSq();
        //         const float min_dist = object_1.radius + object_2.radius;
        //         // Check overlapping
        //         if (dist < min_dist * min_dist)
        //         {
        //             dist = sqrt(dist);
        //             const sf::Vector2f n = v.normalized();
        //             const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
        //             const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
        //             const float delta = 0.5f * response_coef * (dist - min_dist);
        //             // Update positions
        //             object_1.position -= n * (mass_ratio_2 * delta);
        //             object_2.position += n * (mass_ratio_1 * delta);
        //         } }); });

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

    void apply_constraints()
    {
        // for (auto &object : m_objects)
        // {
        //     const auto v = m_constraint_center - object.position;
        //     const float distance = sqrt(v.x * v.x + v.y * v.y);
        //     if (distance > (m_constraint_radius - object.radius))
        //     {
        //         const auto n = v / distance;
        //         object.position = m_constraint_center - n * (m_constraint_radius - object.radius);
        //     }
        // }

        // rewritten with std::for_each and std::execution::par
        std::for_each(std::execution::par_unseq, m_objects.begin(), m_objects.end(), [&](VerletObject &object)
                      {
                          const auto v = m_constraint_center - object.position;
                          const float distance = sqrt(v.x * v.x + v.y * v.y);
                          if (distance > (m_constraint_radius - object.radius))
                          {
                              const auto n = v / distance;
                              object.position = m_constraint_center - n * (m_constraint_radius - object.radius);
                          } });
    }

    void update_objects(float dt)
    {
        for (auto &object : m_objects)
        {
            object.position_update(dt);
        }
    }

public:
    VerletSolver() = default;

    VerletObject &add_object(sf::Vector2f position, float radius)
    {
        return m_objects.emplace_back(position, radius);
    }

    void update(float _dt)
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

    void set_substeps(unsigned _substeps)
    {
        m_substeps = _substeps;
    }

    void set_constraints(sf::Vector2f _postition, float _radius)
    {
        m_constraint_center = _postition;
        m_constraint_radius = _radius;
    }

    void clear_objects()
    {
        m_objects.clear();
    }

    const std::vector<VerletObject> &get_objects()
    {
        return m_objects;
    }

    const sf::CircleShape &get_constraint()
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
};