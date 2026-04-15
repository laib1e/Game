#pragma once
#include <chrono>
#include <random>
#include "Player.h"
#include "Bomber.h"
#include "Bullet.h"

enum class GameState { Playing, GameOver };

class Game
{
private:
    GameState state;
    Renderer renderer;
    Player player;
    std::vector<Bomber> bombers;
    std::vector<Bullet> bullets;
    std::mt19937 rang_gen;

    int screenWidth = 0, screenHeight = 0;
    std::chrono::steady_clock::time_point timestamp;
    float deltaTime = 0;
    float shootTimer = 0;
    float spawnTimer = 3.0f;
    int score = 0;

public:
    explicit Game(int width, int height);
    void frame();
    void onTouch(int action, float y);
};