#pragma once
#include "GameObject.h"
#include "Renderer.h"

class Fighter : public GameObject
{
private:
    float maxSpeed = 150.0f;
    bool wantsToShoot = false;
    float shootTimer = 1.5f;

public:
    explicit Fighter(float x, float y, int width, int height);
    void update(float dt) override;
    void render(Renderer &renderer) override;
    bool isActive() const;
    float getX() const;
    float getY() const;
    int getWidth() const;
    int getHeight() const;
    bool getWantsToShoot() const;
    void setWantsToShoot(bool set);
};
