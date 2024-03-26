#pragma once

#include "../GameObject.hpp"
#include "../Entities/Entities.hpp"
#include "../Util/Gizmo.hpp"
#include "../Util/Grid.hpp"
#include "../Globals.hpp"
#include "../Util/Collision.hpp"
#include "../Util/Quadtree.hpp"
#include <random>


class Scene : public GameObject
{
private:
    static Scene *instance;
    std::vector<std::vector<Box>> grid;
    std::vector<Circle> vehicles;
    Quad quadtree;

private:
    Scene();
    ~Scene();

public:
    // Delete copy constructor and assignment operator to prevent cloning
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    static Scene *getInstance();

    void update(float dt) override;
    void update(sf::Vector2f &desired, float dt);
    void render(sf::RenderTarget *target) override;

    void generateVehicles(sf::Vector2f &position);
};