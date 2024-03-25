#include "../../include/Game/Scene.hpp"

Scene *Scene::instance = nullptr;

Scene::Scene()
{
    if (GLOBAL::display_grid)
    {
        configureGrid(GLOBAL::cell_size, &this->grid);
    }
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
        vehicle.update(desired, vehicles, dt);
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
    Circle vehicle = Circle(1.f, position);
    this->vehicles.push_back(vehicle);
}