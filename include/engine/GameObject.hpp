#pragma once
#include <vector>
#include <memory>
#include <type_traits>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Component.hpp"

class GameObject {

    std::vector<std::unique_ptr<Component>> components;
    public:

    bool alive = true;

    template <typename T, typename... Args>
    T* addComponent(Args&&... args){
        static_assert(std::is_base_of_v<Component, T>);
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->owner = this;
        T* ptr = comp.get();
        components.push_back(std::move(comp));
        ptr->init();
        return ptr;
    }

    template <typename T>
    T* getComponent(){
        for(auto& c : components)
            if(T* casted = dynamic_cast<T*>(c.get()))
                return casted;
        return nullptr;
    }

    void update(float dt){
        for(auto& c : components)
            c->update(dt);
    }

    void render(sf::RenderWindow& w){
        for(auto& c : components)
            c->render(w);
    }
};