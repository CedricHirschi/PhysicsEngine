#include "user.hpp"

#include "solver.hpp"

struct VerletSolver solver;

sf::Clock spawn_clock;
sf::Clock total_clock;

#if LOG
// file for writing number of objects and fps
std::ofstream file;
#endif

float randfloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int randint(int min, int max)
{
    return min + rand() % (max - min);
}

static sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * 3.14f);
    const float b = sin(t + 0.66f * 2.0f * 3.14f);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

void show_statistics(sf::RenderWindow &window, struct VerletSolver &solver, sf::Font &font, float dt);
void spawn_objects(struct VerletSolver &solver, float dt);

sf::Font font;
bool spawn = true;

void User::OnStartup(sf::RenderWindow &window)
{
    window.setFramerateLimit(240);

    solver.set_constraints(2, {window.getSize().x / 2.0f, window.getSize().y / 2.0f}, std::min(window.getSize().x, window.getSize().y) / 2.50f);
    solver.set_substeps(16);

    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
    }
}

void User::OnUpdate(sf::RenderWindow &window, float dt)
{
    solver.update(dt);

    // if mouse is pressed, spawn a new object
    if (spawn_clock.getElapsedTime().asSeconds() > 0.1f && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        spawn_clock.restart();
        auto position = sf::Vector2f(sf::Mouse::getPosition(window));
        auto &object = solver.add_object(position, randfloat(3.0f, 15.0f));
        object.set_color(getRainbow(total_clock.getElapsedTime().asSeconds()));
    }

    // spawn new object every 0.5 seconds
    spawn_objects(solver, dt);

    // show_statistics(window, solver, font, dt);

    window.draw(solver.get_constraint());
    for (auto &object : solver.get_objects())
    {
        sf::CircleShape shape(object.radius);
        shape.setFillColor(object.color);
        shape.setOrigin({object.radius, object.radius});
        shape.setPosition(object.position);
        window.draw(shape);
    }
}

void User::OnEvent(sf::RenderWindow &window, sf::Event &event)
{
    // std::cout << "Event: " << events[event.type] << std::endl;
    switch (event.type)
    {
    case sf::Event::KeyPressed:
        // std::cout << "\tKey: " << event.key.code << std::endl;
        auto key = event.key.code;
        if (key == sf::Keyboard::S)
        {
            spawn = !spawn;
        }
        else if (key == sf::Keyboard::R)
        {
            solver.clear_objects();
        }
        break;
    }
}

void show_statistics(sf::RenderWindow &window, struct VerletSolver &solver, sf::Font &font, float dt)
{
    static std::vector<float> last_fps;
    last_fps.push_back(1.0f / dt);
    if (last_fps.size() > 100)
    {
        last_fps.erase(last_fps.begin());
    }

    sf::Text fps(font, "FPS: " + std::to_string(int(std::accumulate(last_fps.begin(), last_fps.end(), 0.0f) / last_fps.size())), 20);
    fps.setFillColor(sf::Color::White);
    fps.setPosition({10.0f, 10.0f});
    window.draw(fps);

    sf::Text numobj(font, "Objects: " + std::to_string(solver.get_objects().size()), 20);
    numobj.setFillColor(sf::Color::White);
    numobj.setPosition({10.0f, 30.0f});
    window.draw(numobj);
}

void spawn_objects(struct VerletSolver &solver, float dt)
{
    static sf::Clock spawn_clock;
    static sf::Clock total_clock;

    if (spawn_clock.getElapsedTime().asSeconds() > 0.1f && spawn)
    {
        spawn_clock.restart();
        auto position = solver.get_constraint().getPosition() - sf::Vector2f(0.0f, solver.get_constraint().getGlobalBounds().height / 2.5f);
        auto angle = 3.14f / 2.0f * sin(total_clock.getElapsedTime().asSeconds());
        auto velocity = sf::Vector2f(100.0f * sin(angle), 100.0f * cos(angle));
        auto &object = solver.add_object(position, randfloat(3.0f, 15.0f));
        object.set_velocity(velocity, dt);
        object.set_color(getRainbow(total_clock.getElapsedTime().asSeconds()));
    }
}
