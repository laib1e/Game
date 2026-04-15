#include "Player.h"

Player::Player(float x, float y, int screenWidth, int screenHeight, int width, int height)
{
    X = x; Y = y;
    Width = width; Height = height;
    screenWidth_ = screenWidth; screenHeight_ = screenHeight;
    Active = true;
}

void Player::setTargetDirection(float direction)
{
    targetY = direction;
}

void Player::update(float dt)
{
    float desired = targetY - Y;
    VY += (desired - VY) * smoothing * dt;

    if (VY > maxSpeed) VY = maxSpeed;
    if (VY < -maxSpeed) VY = -maxSpeed;

    Y += VY * dt;
    if (Y <= 0) setActive(false);
    if (Y + (float)Height > (float)screenHeight_) Y = (float)screenHeight_ - (float)Height;
}

void Player::render(Renderer &renderer)
{
    renderer.drawRect(X, Y, Width, Height, 0.0f, 1.0f, 0.0);
}


