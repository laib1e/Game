#include "Meteorite.h"

Meteorite::Meteorite(float x, float y, int width, int height)
{
    X = x; Y = y;
    Width = width; Height = height;

    Active = true;
}

void Meteorite::update(float dt)
{
    Y -= maxSpeed * dt;
    if (Y + (float)Height < 0) Active = false;
}

void Meteorite::render(Renderer &renderer)
{
    renderer.drawRect(X, Y, Width, Height, 0.0f, 0.0f, 1.0);
}

bool Meteorite::isActive() const
{
    return Active;
}