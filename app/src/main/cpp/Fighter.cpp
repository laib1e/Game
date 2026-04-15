#include "Fighter.h"

Fighter::Fighter(float x, float y, int width, int height)
{
    X = x; Y = y;
    Width = width; Height = height;

    Active = true;
}

void Fighter::update(float dt)
{
    shootTimer -= dt;
    if (shootTimer <= 0)
    {
        wantsToShoot = true;
        shootTimer = 1.5f;
    }
    X -= maxSpeed * dt;
    if (X + (float)Width < 0) Active = false;
}

void Fighter::render(Renderer &renderer)
{
    renderer.drawRect(X, Y, Width, Height, 1.0f, 0.0f, 0.0);
}

bool Fighter::getWantsToShoot() const
{
    return wantsToShoot;
}

void Fighter::setWantsToShoot(bool set)
{
    wantsToShoot = set;
}
