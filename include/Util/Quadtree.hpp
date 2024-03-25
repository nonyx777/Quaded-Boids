#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../GameObject.hpp"
#include "../Entities/Entities.hpp"
#include "Collision.hpp"

// The objects that we want stored in the quadtree
struct Node
{
    sf::Vector2f pos;
    int data;
    Node(sf::Vector2f _pos, int _data)
    {
        pos = _pos;
        data = _data;
    }
    Node() { data = 0; }
};

class Quad
{
public:
    sf::Vector2f topLeft;
    sf::Vector2f botRight;
    Circle *node;
    Box box;

    Quad *ne;
    Quad *nw;
    Quad *sw;
    Quad *se;

    Quad();
    Quad(sf::Vector2f topLeft, sf::Vector2f botRight);
    Collision collision;

    void insert(Circle *node);
    std::vector<Circle *> search(sf::Vector2f topLeft, sf::Vector2f botRight);
    void clear();
};