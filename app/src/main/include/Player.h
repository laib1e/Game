#pragma once
#include "GameObject.h"
#include "Renderer.h"

class Player : public GameObject
{
private:
    int screenWidth_ = 0, screenHeight_ = 0;
    float maxSpeed = 30.0f;
    float smoothing = 1.5;
    float targetY = 0;

public:
    explicit Player(float x, float y, int screenWidth, int screenHeight,  int width, int height);
    void setTargetDirection(float direction);
    void update(float dt) override;
    void render(Renderer &renderer) override;
    float getX() const;
    float getY() const;
    void setX(float x);
    void setY(float y);
    int getWidth() const;
    int getHeight() const;
    bool isActive() const;
};
