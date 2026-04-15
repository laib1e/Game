#pragma once
#include "GameObject.h"
#include "Renderer.h"

class Bomber : public GameObject
{
private:
    float maxSpeed = 150.0f;

public:
    explicit Bomber(float x, float y, int width, int height);
    void update(float dt) override;
    void render(Renderer &renderer) override;
};
