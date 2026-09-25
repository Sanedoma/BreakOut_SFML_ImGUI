#pragma once
#include "Transform.hpp"

namespace Collision {

    // Renvoie true si les deux rectangles (position = coin haut-gauche) se chevauchent.
    inline bool aabb(const Transform& a, const Transform& b) {
        return a.position.x < b.position.x + b.size.x &&
               a.position.x + a.size.x > b.position.x &&
               a.position.y < b.position.y + b.size.y &&
               a.position.y + a.size.y > b.position.y;
    }
}