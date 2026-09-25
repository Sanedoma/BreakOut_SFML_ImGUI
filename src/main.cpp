#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>

#include "engine/GameObject.hpp"
#include "engine/Transform.hpp"
#include "engine/RectRenderer.hpp"
#include "game/PaddleController.hpp"
#include "game/BallPhysics.hpp"
#include "game/PaddleBounce.hpp"
#include "game/Brick.hpp"
#include "game/BrickCollision.hpp"

void removeDeadEntities(std::vector<std::unique_ptr<GameObject>>& entities){
    entities.erase(
        std::remove_if(entities.begin(), entities.end(), [](const std::unique_ptr<GameObject>& e) {return !e->alive;}),
        entities.end()
    );
}

int main(){

    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Breakout");
    window.setFramerateLimit(60);

    std::vector<std::unique_ptr<GameObject>> entities;

    // Paddle
    auto& paddle = *entities.emplace_back(std::make_unique<GameObject>());
    paddle.addComponent<Transform>(sf::Vector2f{350.f, 560.f}, sf::Vector2f{100.f, 20.f});
    paddle.addComponent<RectRenderer>(sf::Color::White);
    paddle.addComponent<PaddleController>(500.f, 800.f);

    // Ball
    auto& ball = *entities.emplace_back(std::make_unique<GameObject>());
    ball.addComponent<Transform>(sf::Vector2f{400.f, 300.f}, sf::Vector2f{15.f, 15.f});
    ball.addComponent<RectRenderer>(sf::Color::Yellow);
    ball.addComponent<BallPhysics>(sf::Vector2f{200.f, -250.f}, 800.f, 600.f);

    // --- Génération de la grille de briques ---
    const int   brickCols    = 10;
    const int   brickRows    = 5;
    const float brickW       = 70.f;
    const float brickH       = 25.f;
    const float brickGap     = 5.f;    // espace entre briques
    const float gridOffsetX  = 15.f;   // marge à gauche
    const float gridOffsetY  = 50.f;   // marge en haut

    for (int row = 0; row < brickRows; ++row) {
        for (int col = 0; col < brickCols; ++col) {
            float x = gridOffsetX + col * (brickW + brickGap);
            float y = gridOffsetY + row * (brickH + brickGap);

            auto& brick = *entities.emplace_back(std::make_unique<GameObject>());
            brick.addComponent<Transform>(sf::Vector2f{x, y}, sf::Vector2f{brickW, brickH});
            brick.addComponent<RectRenderer>(sf::Color(200, 100, 100));
            brick.addComponent<Brick>(1);
        }
    }

    sf::Clock clock;
    while(window.isOpen()){

        float dt = clock.restart().asSeconds();

        while(const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        for( auto& e : entities)
            e->update(dt);
        
        resolvePaddleBounce(ball, paddle);
        resolveBrickCollisions(ball, entities);
        removeDeadEntities(entities);

        window.clear(sf::Color(30, 30, 40));
        for(auto& e : entities)
            e->render(window);
        window.display();
    }

    return 0;
}