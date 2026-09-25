#pragma once
#include "../engine/Component.hpp"

class Brick : public Component{
public:
    int hitPoints;

    Brick(int hp = 1): hitPoints(hp){}
    
    bool hit(){
        hitPoints--;
        return hitPoints <= 0;
    }
};