#include "Bullet.h"

Bullet::Bullet(float x, float y, float speed, BulletOwner owner)
{
    X = x; Y = y;
    Width = 4; Height = 4;

    VX = speed;
    owner_ = owner;
    Active = true;
}

void Bullet::update(float dt)
{
    X += VX * dt;
    if (X > 5000 || X + (float)Width < 0) Active = false;
}

void Bullet::render(Renderer &renderer)
{
    renderer.drawRect(X, Y, Width, Height, 1.0f, 0.0f, 1.0);
}

BulletOwner Bullet::getOwner() const
{
    return owner_;
}