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
    // sf::Vector2f wander = this->wander();
    // sf::Vector2f seek = this->seek(desired);
    sf::Vector2f separate = this->separate(vehicles);
    sf::Vector2f align = this->align(vehicles);
    sf::Vector2f cohesion = this->cohesion(vehicles);
    // seek *= 1.f;
    separate *= 0.5f;
    align *= 1.5f;
    cohesion *= 1.5f;
    this->edge();
    this->linearAcceleration += separate + align + cohesion;
    this->linearEuler(&this->property);

    if (this->linearVelocity.x > maxSpeed || this->linearVelocity.y > maxSpeed)
    {
        this->linearVelocity = Math::_normalize(this->linearVelocity);
        this->linearVelocity *= maxSpeed;
    }
}

void Circle::render(sf::RenderTarget *target)
{
    target->draw(this->property);
}

sf::Vector2f Circle::seek(sf::Vector2f &desired_)
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

    return steer;
}

sf::Vector2f Circle::separate(std::vector<Circle> &vehicles)
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
        return steer;
    }
    else
    {
        return sf::Vector2f(0.f, 0.f);
    }
}

sf::Vector2f Circle::align(std::vector<Circle> &vehicles)
{
    float align_value = 50.f;
    sf::Vector2f sum;
    int count = 0;
    for (Circle &vehicle : vehicles)
    {
        float d = Math::_length(this->property.getPosition() - vehicle.property.getPosition());
        if (((int)d > 0) && d < align_value)
        {
            sum += vehicle.linearVelocity;
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
        return steer;
    }
    else
    {
        return sf::Vector2f(0.f, 0.f);
    }
}

sf::Vector2f Circle::cohesion(std::vector<Circle> &vehicles)
{
    float center_value = 50.f;
    sf::Vector2f sum;
    int count = 0;
    for (Circle &vehicle : vehicles)
    {
        float d = Math::_length(this->property.getPosition() - vehicle.property.getPosition());
        if (((int)d > 0) && d < center_value)
        {
            sum += vehicle.property.getPosition();
            count++;
        }
    }
    if (count > 0)
    {
        sum /= (float)count;

        return seek(sum);
    }
    else
    {
        return sf::Vector2f(0.f, 0.f);
    }
}

sf::Vector2f Circle::wander()
{
    // float radius = 5.f;
    // sf::Vector2f future_pos = (this->property.getPosition() + this->linearVelocity) * 2.f;

    // int random_angle = random(-360, 360);
    // float x = radius * Math::_cos(random_angle);
    // float y = radius * Math::_sin(random_angle);

    // sf::Vector2f coord = sf::Vector2f(x, y);
    // coord += future_pos;

    // return coord;
}

void Circle::edge()
{
    sf::Vector2f position = this->property.getPosition();
    sf::Vector2f desired;

    if (position.x < 10.f)
    {
        desired = sf::Vector2f(maxSpeed, this->linearVelocity.y);
        sf::Vector2f steer = desired - this->linearVelocity;
        steer *= 0.5f;
        this->linearAcceleration += steer;
    }
    if (position.x > GLOBAL::window_width - 10.f)
    {
        desired = sf::Vector2f(-maxSpeed, this->linearVelocity.y);
        sf::Vector2f steer = desired - this->linearVelocity;
        steer *= 0.5f;
        this->linearAcceleration += steer;
    }
    if (position.y < 10.f)
    {
        desired = sf::Vector2f(this->linearVelocity.x, maxSpeed);
        sf::Vector2f steer = desired - this->linearVelocity;
        steer *= 0.5f;
        this->linearAcceleration += steer;
    }
    if (position.y > GLOBAL::window_height - 10.f)
    {
        desired = sf::Vector2f(this->linearVelocity.x, -maxSpeed);
        sf::Vector2f steer = desired - this->linearVelocity;
        steer *= 0.5f;
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