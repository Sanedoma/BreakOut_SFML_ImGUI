#include "engine/Game.hpp"
#include "engine/Transform.hpp"
#include "engine/RectRenderer.hpp"
#include "game/PaddleController.hpp"
#include "game/BallPhysics.hpp"
#include "game/Brick.hpp"
#include "game/PaddleBounce.hpp"
#include "game/BrickCollision.hpp"
#include <algorithm>
#include <string>
#include <iostream>

// --- Helpers libres (repris de main.cpp) ---

namespace {
    void removeDeadEntities(std::vector<std::unique_ptr<GameObject>>& entities){
        entities.erase(
            std::remove_if(entities.begin(), entities.end(), [](const std::unique_ptr<GameObject>& e) {return !e->alive;}),
            entities.end()
        );
    }

    bool bricksRemaining(std::vector<std::unique_ptr<GameObject>>& entities) {
        for (auto& e : entities)
            if (e->getComponent<Brick>() && e->alive)
                return true;
        return false;
    }
}

Game::Game() : window(sf::VideoMode({800u, 600u}), "Breakout"), scoreText(font), livesText(font), messageText(font){
    window.setFramerateLimit(60); 

    if(!font.openFromFile("assets/fonts/hunter-x-hunter-jap-sans-serif.otf"))
        std::cerr << "Erreur : Police introuvable\n";

    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({10.f, 10.f});

    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition({650.f, 10.f});

    messageText.setCharacterSize(48);

    resetGame();
}

void Game::resetGame(){
    entities.clear();
    score = 0;
    lives = 3;
    state = State::Playing;

    //Paddle
    paddle = entities.emplace_back(std::make_unique<GameObject>()).get();
    paddle->addComponent<Transform>(sf::Vector2f{350.f, 560.f}, sf::Vector2f{100.f, 20.f});
    paddle->addComponent<RectRenderer>(sf::Color::White);
    paddle->addComponent<PaddleController>(500.f, 800.f);

    //Balle
    ball = entities.emplace_back(std::make_unique<GameObject>()).get();
    ball->addComponent<Transform>(sf::Vector2f{400.f, 300.f}, sf::Vector2f{15.f, 15.f});
    ball->addComponent<RectRenderer>(sf::Color::Yellow);
    ball->addComponent<BallPhysics>(sf::Vector2f{200.f, -250.f}, 800.f, 600.f);

    //Placement des briques
    const int brickCols = 10; 
    const int brickRows = 5;
    const float brickW = 70.f, brickH = 25.f, gap = 5.f, offX = 15.f, offY = 50.f;
    for(int row = 0; row < brickRows; ++row){
        for(int col = 0; col < brickCols; ++col){
            float x = offX + col * (brickW + gap);
            float y = offY + row * (brickH + gap);

            auto& b = *entities.emplace_back(std::make_unique<GameObject>());
            b.addComponent<Transform>(sf::Vector2f{x, y}, sf::Vector2f{brickW, brickH});
            b.addComponent<RectRenderer>(sf::Color(200, 100, 100));
            b.addComponent<Brick>(1);
        }
    }
}

// --- Boucle de Jeu ---
void Game::run(){
    sf::Clock clock;
    while(window.isOpen()){
        float dt = clock.restart().asSeconds();

        processEvents();
        Update(dt);
        render();
    }
}


// --- Événements : fermeture + rejouer ---
void Game::processEvents(){
    while(const std::optional event = window.pollEvent()){
        if(event->is<sf::Event::Closed>())
            window.close();
    }

    // Rejouer : seulement en fin de partie, sur touche R
    if (state == State::GameOver || state == State::Win) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
            resetGame();
    }
}

void Game::Update(float dt){
    if(state != State::Playing)
        return;

    for(auto& e : entities)
        e->update(dt);

    resolvePaddleBounce(*ball, *paddle);
    score += resolveBrickCollisions(*ball, entities);
    removeDeadEntities(entities);

    auto* physics = ball->getComponent<BallPhysics>();
    if (physics && physics->hasFellOff()) {
        lives--;
        if (lives > 0) {
            // replace la balle
            auto* t = ball->getComponent<Transform>();
            t->position = {400.f, 300.f};
            physics->setVelocity({200.f, -250.f});
            physics->resetFellOff();
        } else {
            state = State::GameOver;   // transition d'état, plus de window.close()
        }
    }

    // Victoire ?
    if (!bricksRemaining(entities))
        state = State::Win;

}

// --- Rendu : dépend de l'états du jeu ---
void Game::render() {
    window.clear(sf::Color(30, 30, 40));

    for (auto& e : entities)
        e->render(window);

    scoreText.setString("Score : " + std::to_string(score));
    livesText.setString("Vies : " + std::to_string(lives));
    window.draw(scoreText);
    window.draw(livesText);

    // Message de fin selon l'état
    if (state == State::GameOver || state == State::Win) {
        messageText.setFillColor(
            state == State::Win ? sf::Color::Green
                                : sf::Color::Red
        );
        messageText.setString(
            state == State::Win ? "VICTOIRE ! (R pour rejouer)"
                                : "GAME OVER (R pour rejouer)"
        );
        messageText.setPosition({150.f, 260.f});
        window.draw(messageText);
    }

    window.display();
}