#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

class GameObject;

class Component {
public:
    GameObject* owner = nullptr;

    virtual ~Component() = default;

    virtual void init (){};
    virtual void update(float dt){};
    virtual void render(sf::RenderWindow&){};
};