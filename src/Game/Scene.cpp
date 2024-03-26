#include "../../include/Game/Scene.hpp"

Scene *Scene::instance = nullptr;

Scene::Scene()
{
    if (GLOBAL::display_grid)
    {
        configureGrid(GLOBAL::cell_size, &this->grid);
    }
    quadtree = Quad(sf::Vector2f(0.f, 0.f), sf::Vector2f(GLOBAL::window_width, GLOBAL::window_height));
}

Scene::~Scene()
{
    delete instance;
}

Scene *Scene::getInstance()
{
    if (!instance)
        instance = new Scene();

    return instance;
}

void Scene::update(float dt)
{
    //..
}

void Scene::update(sf::Vector2f &desired, float dt)
{
    for (Circle &vehicle : this->vehicles)
        quadtree.insert(&vehicle);

    for (Circle &vehicle : this->vehicles)
    {
        sf::Vector2f topleft = sf::Vector2f(
            vehicle.property.getPosition().x - GLOBAL::range_offset,
            vehicle.property.getPosition().y - GLOBAL::range_offset);
        sf::Vector2f botright = sf::Vector2f(
            vehicle.property.getPosition().x + GLOBAL::range_offset,
            vehicle.property.getPosition().y + GLOBAL::range_offset);
        
        std::vector<Circle*> in_range = quadtree.search(topleft, botright);
        vehicle.update(desired, in_range, dt);
    }

    quadtree.clear();
}

void Scene::render(sf::RenderTarget *target)
{
    for (Circle &vehicle : this->vehicles)
        vehicle.render(target);

    if (this->grid.size() > 0)
    {
        for (uint i = 0; i < grid.size(); i++)
        {
            for (uint j = 0; j < grid[i].size(); j++)
            {
                target->draw(grid[i][j].property);
            }
        }
    }
}

void Scene::generateVehicles(sf::Vector2f &position)
{
    Circle vehicle = Circle(GLOBAL::size, position);
    float x = Math::random(-3, 3);
    float y = Math::random(-3, 3);
    vehicle.linearVelocity = sf::Vector2f(x, y);

    this->vehicles.push_back(vehicle);
    GLOBAL::boids = this->vehicles.size();
}