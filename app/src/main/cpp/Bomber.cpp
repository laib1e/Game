#include "Bomber.h"

Bomber::Bomber(float x, float y, int width, int height)
{
    X = x; Y = y;
    Width = width; Height = height;

    Active = true;
}

void Bomber::update(float dt)
{
    X -= maxSpeed * dt;
    if (X + (float)Width < 0) Active = false;
}

void Bomber::render(Renderer &renderer)
{
    renderer.drawRect(X, Y, Width, Height, 0.0f, 0.0f, 1.0);
}

bool Bomber::isActive() const
{
    return Active;
}