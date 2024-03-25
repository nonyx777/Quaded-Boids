#include "../../include/Entities/Circle.hpp"

Circle::Circle()
{
    this->initVariables();
}

Circle::Circle(float radius)
{
    this->initVariables();
    this->property.setRadius(radius);
    this->property.setOrigin(sf::Vector2f(radius, radius));
}

Circle::Circle(float radius, sf::Vector2f position)
{
    this->initVariables();
    this->property.setRadius(radius);
    this->property.setOrigin(sf::Vector2f(radius, radius));
    this->property.setPosition(position);
}

void Circle::initVariables()
{
    this->property.setFillColor(sf::Color::White);
    this->inverseMass = this->mass == 0.f ? 100000.f : 1.f / this->mass;
}

void Circle::calcMomentOfInertia()
{
    // mr^2
    this->momentOfInertia = this->mass * (this->property.getRadius() * this->property.getRadius());
}

void Circle::update(float dt)
{
    //...
}

void Circle::update(sf::Vector2f &desired, std::vector<Circle> &vehicles, float dt)
{
    this->steer(desired);
    this->separate(vehicles);
    this->linearEuler(&this->property);
}

void Circle::render(sf::RenderTarget *target)
{
    target->draw(this->property);
}

void Circle::steer(sf::Vector2f &desired_)
{
    // multiplied by 1.01 because of a bug when desired and vehicle position
    // is the same when the vehicle is created
    sf::Vector2f desired = (desired_ * 1.01f) - this->property.getPosition();
    float d = Math::_length(desired);
    desired = Math::_normalize(desired);

    if (d < this->minDistance)
    {
        float m = this->map(d, this->minDistance, this->maxSpeed);
        desired *= m;
    }
    else
    {
        desired *= maxSpeed;
    }

    sf::Vector2f steer = desired - this->linearVelocity;
    steer *= maxForce;

    this->linearAcceleration += steer;
}

void Circle::separate(std::vector<Circle> &vehicles)
{
    float separation = this->property.getRadius() * 2.f;
    sf::Vector2f sum;
    int count = 0;
    for (Circle &vehicle : vehicles)
    {
        float d = Math::_length(this->property.getPosition() - vehicle.property.getPosition());
        if (((int)d > 0) && d < separation)
        {
            sf::Vector2f diff = this->property.getPosition() - vehicle.property.getPosition();
            diff = Math::_normalize(diff);
            diff /= d; // weight
            sum += diff;
            count++;
        }
    }
    if (count > 0)
    {
        sum /= (float)count;
        sum = Math::_normalize(sum);
        sum *= maxSpeed;
        sf::Vector2f steer = sum - this->linearVelocity;
        steer *= maxForce;
        this->linearAcceleration += steer;
    }
}

sf::Vector2f Circle::limit(sf::Vector2f steer_, float max)
{
    if (Math::_dot(steer_, steer_) < max * max)
    {
        return steer_;
    }

    steer_ = Math::_normalize(steer_);
    steer_ *= this->maxForce;
    return steer_;
}

float Circle::map(float d, float max1, float max2)
{
    float value = (d * max2) / max1;
    return value;
}