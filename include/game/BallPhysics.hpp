#pragma once
#include "../engine/Component.hpp"
#include "../engine/GameObject.hpp"
#include "../engine/Transform.hpp"

class BallPhysics : public Component {
    Transform* transform = nullptr;
    sf::Vector2f velocity;
    float windowWidth;
    float windowHeight;

public:
    BallPhysics(
        sf::Vector2f vel = {200.f, -250.f},
        float winWidth = 800.f,
        float winHeigth = 600.f
    ) : velocity(vel), windowWidth(winWidth), windowHeight(winHeigth) {}

    void init() override{
        transform = owner->getComponent<Transform>();
    }

    void update(float dt){
        if(!transform) return;

        transform->position += velocity * dt;

        if(transform->position.x < 0.f){
            transform->position.x = 0.f;
            velocity.x = -velocity.x;
        }

        if(transform->position.x + transform->size.x > windowWidth){
            transform->position.x = windowWidth - transform->size.x;
            velocity.x = -velocity.x;
        }

        if(transform->position.y < 0.f){
            transform->position.y = 0.f;
            velocity.y = -velocity.y;
        }

        if (transform->position.y + transform->size.y > windowHeight) {
            transform->position.y = windowHeight - transform->size.y;
            velocity.y = -velocity.y;
        }
    }

        // Accès pour le système de collision extérieur
        sf::Vector2f getVelocity() const { return velocity; }
        void setVelocity(sf::Vector2f v) { velocity = v; }
        Transform* getTransform() const { return transform; }

};