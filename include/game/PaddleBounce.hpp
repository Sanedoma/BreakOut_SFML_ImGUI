#pragma once
#include <cmath>
#include "../engine/Collision.hpp"
#include "../engine/GameObject.hpp"
#include "../engine/Transform.hpp"
#include "BallPhysics.hpp"

inline void resolvePaddleBounce(GameObject& ballObj, GameObject& paddleObj){

    auto* physics  = ballObj.getComponent<BallPhysics>();
    auto* ballT    = ballObj.getComponent<Transform>();
    auto* paddleT  = paddleObj.getComponent<Transform>();

    if(!Collision::aabb(*ballT, *paddleT)) return;

    sf::Vector2f v = physics->getVelocity();

    if(v.y <= 0.f) return;

    float ballCenterX = ballT->position.x + (ballT->size.x / 2);
    float paddleCenterX = paddleT->position.x + (paddleT->size.x / 2);

    float offset = (ballCenterX - paddleCenterX) / (paddleT->size.x / 2.f);

    float speed = std::sqrt(v.x * v.x + v.y * v.y);

    float maxBounceAngle = 1.0f;
    float angleFactor = offset * maxBounceAngle;

    v.x = speed * std::sin(angleFactor);
    v.y = -speed * std::cos(angleFactor);

    physics->setVelocity(v);
    ballT->position.y = paddleT->position.y - ballT->size.y;
 
}