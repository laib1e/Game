#include "Game.h"
#include <algorithm>
#include <android/log.h>
#define LOG(...) __android_log_print(ANDROID_LOG_DEBUG, "Game", __VA_ARGS__)

Game::Game(int width, int height) :
          renderer(width, height),
          player(100, (float)height / 2.0f, width, height, 60, 40),
          screenWidth(width),
          screenHeight(height),
          timestamp(std::chrono::steady_clock::now()),
          rang_gen(std::random_device{}()),
          state(GameState::Playing)
{}

void Game::frame()
{
    if (state == GameState::GameOver)
    {
        // Game Over
        renderer.drawFrame();
        player.render(renderer);
        for(Bird& bird : birds)
        {
            bird.render(renderer);
            bird.setActive(false);
        }

        for (Fighter& fighter : fighters)
        {
            fighter.render(renderer);
            fighter.setActive(false);
        }

        for (Bullet& bullet : bullets)
        {
            bullet.render(renderer);
            bullet.setActive(false);
        }

        for (Bomber& bomber : bombers)
        {
            bomber.render(renderer);
            bomber.setActive(false);
        }
        return;
    }
    auto timeframe = std::chrono::steady_clock::now();
    deltaTime = static_cast<decltype(deltaTime)>((timeframe - timestamp).count()) / std::chrono::steady_clock::period::den;
    timestamp = timeframe;

    gameTime += deltaTime;
    maxBombers = 1 + (int)(gameTime * 0.03f);
    maxFighters = 1 + (int)(gameTime * 0.1f);
    maxBirds = 2 + (int)(gameTime * 0.15f);

    bomberSpawnTimer -= deltaTime;
    if (bomberSpawnTimer <= 0 and (int)bombers.size() < maxBombers)
    {
        std::uniform_real_distribution<float> dist_bombers(screenHeight * 0.66f, screenHeight - bomberHeight);
        bombers.emplace_back(screenWidth, dist_bombers(rang_gen), bomberWidth, bomberHeight);
        bomberSpawnTimer = 1.5f;
    }

    fightersSpawnTimer -= deltaTime;
    if (fightersSpawnTimer <= 0 and (int)fighters.size() < maxFighters)
    {
        std::uniform_real_distribution<float> dist_fighters(screenHeight * 0.33f, screenHeight * 0.66f - fighterHeight);
        fighters.emplace_back(screenWidth, dist_fighters(rang_gen), fighterWidth, fighterHeight);
        fightersSpawnTimer = 3.0f;
    }

    birdSpawnTimer -= deltaTime;
    if (birdSpawnTimer <= 0 and (int)birds.size() < maxBirds)
    {
        std::uniform_real_distribution<float> dist_birds(screenHeight * 0.1f, screenHeight * 0.33f - birdHeight);
        birds.emplace_back(screenWidth, dist_birds(rang_gen), birdWidth, birdHeight);
        birdSpawnTimer = 2.0f;
    }

    meteorSpawnTimer -= deltaTime;
    if (meteorSpawnTimer <= 0 and not meteor)
    {
        std::uniform_real_distribution<float> dist_meteor(meteorWidth, screenWidth);
        meteor = std::unique_ptr<Meteorite>(new Meteorite(dist_meteor(rang_gen), screenHeight, meteorWidth, meteorHeight));
        meteorSpawnTimer = 20.0f;
    }

    for (Fighter& fighter : fighters)
    {
        if (fighter.getWantsToShoot())
        {
            bullets.emplace_back(fighter.getX(),
                                 fighter.getY() + (float)fighter.getHeight() / 2,
                                 -300.0f, BulletOwner::Enemy);
            fighter.setWantsToShoot(false);
        }
    }

    shootTimer -= deltaTime;
    if (shootTimer <= 0)
    {
        bullets.emplace_back(player.getX() + (float)player.getWidth(),
                             player.getY() + (float)player.getHeight() / 2,
                             300.0f, BulletOwner::Player);
        shootTimer = 1.5f;
    }

    if (meteor)
    {
        meteor->update(deltaTime);
        if (meteor->collides(player))
            player.setActive(false);
    }

    for (Bird& bird : birds)
    {
        bird.update(deltaTime);
        if (player.collides(bird))
            player.setActive(false);
    }

    for (Fighter& fighter : fighters)
    {
        fighter.update(deltaTime);
        if (player.collides(fighter))
            player.setActive(false);

        if (meteor and meteor->collides(fighter))
            fighter.setActive(false);
    }

    for (Bomber& bomber : bombers)
    {
        bomber.update(deltaTime);
        if (player.collides(bomber))
            player.setActive(false);

        if (meteor and meteor->collides((bomber)))
            bomber.setActive(false);
    }

    for (Bullet& bullet : bullets)
    {
        bullet.update(deltaTime);

        if (meteor and meteor->collides((bullet)))
            bullet.setActive(false);
    }

    for (Bullet& bullet: bullets)
    {
        if (bullet.getOwner() == BulletOwner::Player)
        {
            for (Bomber& bomber : bombers)
            {
                if (bomber.collides(bullet))
                {
                    bullet.setActive(false);
                    bomber.setActive(false);
                    score += 50;
                }
            }

            for (Fighter& fighter : fighters)
            {
                if (fighter.collides(bullet))
                {
                    fighter.setActive(false);
                    bullet.setActive(false);
                    score += 100;
                }
            }
        }

        if (bullet.getOwner() == BulletOwner::Enemy) {
            if (player.collides(bullet))
            {
                player.setActive(false);
            }
        }
    }

    player.update(deltaTime);
    renderer.drawFrame();
    player.render(renderer);

    birds.erase(
            std::remove_if(birds.begin(), birds.end(),
                           [](const Bird& b) { return !b.isActive(); }),
            birds.end());

    bombers.erase(
            std::remove_if(bombers.begin(), bombers.end(),
                           [](const Bomber& b) { return !b.isActive(); }),
                           bombers.end());

    bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                           [](const Bullet& b) {return !b.isActive(); }),
                           bullets.end()
            );

    fighters.erase(
            std::remove_if(fighters.begin(), fighters.end(),
                           [](const Fighter& b) {return !b.isActive(); }),
            fighters.end()
    );

    for(Bird& bird : birds)
    {
        bird.render(renderer);
    }

    for (Fighter& fighter : fighters)
    {
        fighter.render(renderer);
    }

    for (Bullet& bullet : bullets)
    {
        bullet.render(renderer);
    }

    for (Bomber& bomber : bombers)
    {
        bomber.render(renderer);
    }

    if (meteor)
    {
        meteor->render(renderer);

        if (not meteor->isActive())
        {
            meteor.reset();
        }
    }

    if (not player.isActive())
    {
        state = GameState::GameOver;
    }
}

void Game::onTouch(int action, float y)
{
    player.setTargetDirection((float)screenHeight - y);
}

void Game::reset()
{
    player.setX(100);
    player.setY(screenHeight / 2.0f);
    player.setActive(true);

    score = 0;
    deltaTime = 0;
    shootTimer = 0;

    gameTime = 0;
    bomberSpawnTimer = 1.5f;
    fightersSpawnTimer = 3.0f;
    birdSpawnTimer = 2.0f;
    meteorSpawnTimer = 1000.0f;

    bombers.clear();
    fighters.clear();
    bullets.clear();
    birds.clear();
    meteor.reset();

    state = GameState::Playing;
}
