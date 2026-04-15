#pragma once
#include "GameObject.h"
#include "Renderer.h"

class Meteorite : public GameObject
{
private:
    float maxSpeed = 300.0f;

public:
    explicit Meteorite(float x, float y, int width, int height);
    void update(float dt) override;
    void render(Renderer &renderer) override;
    bool isActive() const;
};
