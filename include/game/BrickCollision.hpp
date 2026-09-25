#pragma once
#include <cmath>
#include <vector>
#include <memory>
#include "../engine/Collision.hpp"
#include "../engine/GameObject.hpp"
#include "../engine/Transform.hpp"
#include "BallPhysics.hpp"
#include "Brick.hpp"

inline void resolveBrickCollisions(
    GameObject& ballObj,
    std::vector<std::unique_ptr<GameObject>>& entities
){
    auto* physics = ballObj.getComponent<BallPhysics>();
    auto* ballT = ballObj.getComponent<Transform>();
    if(!physics  || !ballT) return;

    for(auto& e : entities){
        Brick* brick = e->getComponent<Brick>();
        if(!brick || !e->alive) continue;

        Transform* brickT = e->getComponent<Transform>();
        if(!Collision::aabb(*ballT, *brickT)) continue;

           // --- Déterminer le côté d'impact via la profondeur de recouvrement ---
        float ballCX   = ballT->position.x + ballT->size.x / 2.f;
        float ballCY   = ballT->position.y + ballT->size.y / 2.f;
        float brickCX  = brickT->position.x + brickT->size.x / 2.f;
        float brickCY  = brickT->position.y + brickT->size.y / 2.f;
        // distance entre centres
        float dx = ballCX - brickCX;
        float dy = ballCY - brickCY;
        // recouvrement sur chaque axe = (demi-largeurs additionnées) - distance
        float overlapX = (ballT->size.x / 2.f + brickT->size.x / 2.f) - std::abs(dx);
        float overlapY = (ballT->size.y / 2.f + brickT->size.y / 2.f) - std::abs(dy);

        sf::Vector2f v = physics->getVelocity();

        // Le plus PETIT recouvrement = axe d'entrée = axe à inverser
        if (overlapX < overlapY) {
            v.x = -v.x;                              // impact latéral
        } else {
            v.y = -v.y;                              // impact haut/bas
        }
        physics->setVelocity(v);
        
        if (brick->hit())
            e->alive = false;                        
        break;   // une seule brique par frame : évite les rebonds incohérents
    }
}