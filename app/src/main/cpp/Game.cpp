#include "Game.h"
#include <algorithm>
#include <android/log.h>
#define LOG(...) __android_log_print(ANDROID_LOG_DEBUG, "Game", __VA_ARGS__)

Game::Game(int width, int height)
        : renderer(width, height),
          player(100, (float)height / 2.0f, width, height, 60, 40),
          screenWidth(width),
          screenHeight(height),
          timestamp(std::chrono::steady_clock::now())
{
    state = GameState::Playing;
    std::mt19937 rand_gen(std::random_device{}());
}

void Game::frame()
{
    if (state == GameState::GameOver)
    {
        // Game Over
        renderer.drawFrame();
        player.render(renderer);
        for (Bomber& bomber : bombers)
        {
            bomber.render(renderer);
        }
        return;
    }
    auto timeframe = std::chrono::steady_clock::now();
    deltaTime = static_cast<decltype(deltaTime)>((timeframe - timestamp).count()) / std::chrono::steady_clock::period::den;
    timestamp = timeframe;

    spawnTimer -= deltaTime;
    if (spawnTimer <= 0)
    {
        int bomberWidth = 80;
        int bomberHeight = 60;
        std::uniform_real_distribution<float> dist(screenHeight * 0.5f, screenHeight - bomberHeight);
        bombers.emplace_back(screenWidth, dist(rang_gen), bomberWidth, bomberHeight);
        spawnTimer = 3.0f;
    }

    shootTimer -= deltaTime;
    if (shootTimer <= 0)
    {
        bullets.emplace_back(player.getX() + (float)player.getWidth(),
                             player.getY() + (float)player.getHeight() / 2,
                             screenWidth,
                             150.0f, BulletOwner::Player);
        shootTimer = 0.5f;
    }

    for (Bomber& bomber : bombers)
    {
        bomber.update(deltaTime);
        if (player.collides(bomber))
            state = GameState::GameOver;
    }

    for (Bullet& bullet : bullets)
    {
        bullet.update(deltaTime);
    }

    for (Bullet& bullet: bullets)
    {
        if (bullet.getOwner() != BulletOwner::Player) continue;
        for (Bomber& bomber : bombers)
        {
            if (bomber.collides(bullet))
            {
                bullet.setActive(false);
                bomber.setActive(false);
                score += 100;
            }
        }
    }

    player.update(deltaTime);
    renderer.drawFrame();
    player.render(renderer);

    bombers.erase(
            std::remove_if(bombers.begin(), bombers.end(),
                           [](const Bomber& b) { return !b.isActive(); }),
                           bombers.end());

    bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                           [](const Bullet& b) {return !b.isActive(); }),
                           bullets.end()
            );

    for (Bullet& bullet : bullets)
    {
        bullet.render(renderer);
    }

    for (Bomber& bomber : bombers)
    {
        bomber.render(renderer);
    }
}

void Game::onTouch(int action, float y)
{
    player.setTargetDirection((float)screenHeight - y);
}