#pragma once
#include "GameObject.h"
#include "Renderer.h"

enum class BulletOwner { Player, Enemy };

class Bullet : public GameObject
{
private:
    BulletOwner owner_;

public:
    explicit Bullet(float x, float y, int width, float speed, BulletOwner owner);
    void update(float dt) override;
    void render(Renderer &renderer) override;
    bool isActive() const;
    BulletOwner getOwner() const;
};
