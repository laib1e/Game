#pragma once
#include <random>
#include "GameObject.h"
#include "Renderer.h"

class Bird : public GameObject
{
private:
    float maxSpeed = 30.0f;
    std::mt19937 rand_vy;
    float randTimer = 1.0f;

public:
    explicit Bird(float x, float y, int width, int height);
    void update(float dt) override;
    void render(Renderer &renderer) override;
};
