#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include <iostream>


class RectRenderer : public Component {
    Transform* transform = nullptr;
    public: 
    sf::Color color;

    RectRenderer(sf::Color col = sf::Color::White) : color(col){}

    void init()override {
        transform = owner->getComponent<Transform>();
        /*if (!transform)
            std::cout << "[RectRenderer] AUCUN Transform trouve !\n";
        else
            std::cout << "[RectRenderer] Transform OK\n";*/
    }

    void render(sf::RenderWindow& window) override{
        if(!transform) return;

        sf::RectangleShape shape(transform->size);
        shape.setPosition(transform->position);
        shape.setFillColor(color);
        window.draw(shape);
    }
};