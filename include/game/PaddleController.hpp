#pragma once
#include <SFML/Window/Keyboard.hpp>
#include "../engine/Component.hpp"
#include "../engine/GameObject.hpp"
#include "../engine/Transform.hpp"

class PaddleController : public Component{
    Transform* transform = nullptr;
    float speed;
    float windowWidth;

public:
    PaddleController(float spd = 500.f, float winWidth = 800.f) : speed(spd), windowWidth(winWidth){}

    void init() override{
        transform = owner->getComponent<Transform>();
    }

    void update(float dt) override {
        if (!transform) return;

        float dir = 0.f;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            dir = -1.f;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            dir = 1.f;
        
        transform->position.x += dir * speed * dt;

        if (transform->position.x < 0.f)
            transform->position.x = 0.f;
        if (transform->position.x + transform->size.x > windowWidth)
            transform->position.x = windowWidth - transform->size.x;
    }
};