#include "Bird.h"

Bird::Bird(float x, float y, int width, int height) : rand_vy(std::random_device{}())
{
    X = x; Y = y;
    Width = width; Height = height;

    Active = true;
}

void Bird::update(float dt)
{
    randTimer -= dt;
    if (randTimer <= 0)
    {
        std::uniform_real_distribution<float> dist_vy(-100.0f, 100.0f);
        VY = dist_vy(rand_vy);
        randTimer = 1.0f;
    }
    Y += VY * dt;
    X -= maxSpeed * dt;
    if (X + (float)Width < 0) Active = false;
}

void Bird::render(Renderer &renderer)
{
    renderer.drawRect(X, Y, Width, Height, 1.0f, 1.0f, 1.0);
}
