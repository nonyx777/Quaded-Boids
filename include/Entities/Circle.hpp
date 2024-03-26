#pragma once

#include "../GameObject.hpp"
#include "../Rigidbody.hpp"
#include "Entity.hpp"
#include "../Util/Math.hpp"
#include "../Globals.hpp"
#include <random>

class Circle : public GameObject, public Rigidbody, public Entity
{
public:
    sf::CircleShape property;

public:
    void initVariables();
    float maxSpeed = 4.f;
    float maxForce = 0.03f;
    float minDistance = 200.f;

public:
    Circle();
    Circle(float radius);
    Circle(float radius, sf::Vector2f position);

    void calcMomentOfInertia() override;
    void update(float dt) override;
    void update(sf::Vector2f &desired, std::vector<Circle*> &vehicles, float dt);
    void render(sf::RenderTarget *target) override;

    sf::Vector2f seek(sf::Vector2f &desired);
    sf::Vector2f separate(std::vector<Circle*> &vehicles);
    sf::Vector2f align(std::vector<Circle*> &vehicles);
    sf::Vector2f cohesion(std::vector<Circle*> &vehicles);
    sf::Vector2f wander();
    void edge();

    sf::Vector2f limit(sf::Vector2f steer_, float max);
    float map(float d, float max1, float max2);
};