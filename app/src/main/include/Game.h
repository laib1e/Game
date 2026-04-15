#pragma once
#include <chrono>
#include "Player.h"

class Game
{
private:
    Renderer renderer;
    Player player;

    int screenWidth = 0, screenHeight = 0;
    std::chrono::steady_clock::time_point timestamp;
    float deltaTime = 0;

public:
    explicit Game(int width, int height);
    void frame();
    void onTouch(int action, float y);
};