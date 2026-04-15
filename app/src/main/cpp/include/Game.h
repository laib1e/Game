#pragma once
#include <chrono>
#include <random>
#include <memory>
#include "Player.h"
#include "Bomber.h"
#include "Bullet.h"
#include "Fighter.h"
#include "Bird.h"
#include "Meteorite.h"

enum class GameState { Playing, GameOver };

class Game
{
private:
    GameState state;
    Renderer renderer;
    Player player;
    std::vector<Bomber> bombers;
    std::vector<Fighter> fighters;
    std::vector<Bullet> bullets;
    std::vector<Bird> birds;
    std::unique_ptr<Meteorite> meteor;
    std::mt19937 rang_gen;

    int screenWidth = 0, screenHeight = 0;
    std::chrono::steady_clock::time_point timestamp;
    float deltaTime = 0;
    float shootTimer = 0;

    float gameTime = 0, bomberSpawnTimer = 1.5f, fightersSpawnTimer = 3.0f, birdSpawnTimer = 2.0f, meteorSpawnTimer = 20.0f;
    int maxBombers = 0, maxFighters = 0, maxBirds = 0;

    int playerWidth = 60, bomberWidth = 80, fighterWidth = playerWidth, meteorWidth = 150, birdWidth = 20;
    int playerHeight = 40, bomberHeight = 60, fighterHeight = playerHeight, meteorHeight = 150, birdHeight  = 20;
    int score = 0;

    float resetBtnX = 0, resetBtnY = 0, resetBtnW = 0, resetBtnH = 0;
    float exitBtnX = 0, exitBtnY = 0, exitBtnW = 0, exitBtnH = 0;
    bool shouldExit = false;

    void drawGameOver();
    void drawScore();

public:
    explicit Game(int width, int height);
    void frame();
    void onTouch(int action, float y, float x);
    void reset();

    bool getShouldExit() const;
    Renderer *getRenderer();
};