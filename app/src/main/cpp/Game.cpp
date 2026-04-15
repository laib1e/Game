#include "Game.h"

Game::Game(int width, int height)
        : renderer(width, height),
          player(100, (float)height / 2.0f, width, height, 60, 40),
          screenWidth(width),
          screenHeight(height),
          timestamp(std::chrono::steady_clock::now())
{
}

void Game::frame()
{
    auto timeframe = std::chrono::steady_clock::now();
    deltaTime = static_cast<decltype(deltaTime)>((timeframe - timestamp).count()) / std::chrono::steady_clock::period::den;
    timestamp = timeframe;

    player.update(deltaTime);
    renderer.drawFrame();
    player.render(renderer);
}

void Game::onTouch(int action, float y)
{
    player.setTargetDirection((float)screenHeight - y);
}