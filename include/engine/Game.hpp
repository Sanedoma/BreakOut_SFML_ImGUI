#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "GameObject.hpp"

class Game {
    enum class State {Playing, GameOver, Win};
    
    sf::RenderWindow window;
    sf::Font font;
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text messageText;

    State state = State::Playing;
    std::vector<std::unique_ptr<GameObject>> entities;
    GameObject* ball = nullptr;
    GameObject* paddle = nullptr;

    int score = 0;
    int lives = 3;

    void processEvents();
    void Update(float dt);
    void render();
    void resetGame();

public:
    Game();
    void run();
};