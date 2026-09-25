#pragma once
#include <SFML/System/Vector2.hpp>
#include "Component.hpp"

class Transform : public Component {
    public:

    sf::Vector2f position;
    sf::Vector2f size;

    Transform(sf::Vector2f pos = {0.f, 0.f}, sf::Vector2f sz = {0.f, 0.f}) : position(pos), size(sz){}
};