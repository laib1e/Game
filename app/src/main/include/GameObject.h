#pragma once

class Renderer;

class GameObject
{
protected:
    float X = 0; //pos
    float Y = 0;
    float VX = 0; //speed
    float VY = 0;
    int Width = 0; //size
    int Height = 0;
    bool Active = false;

public:
    virtual ~GameObject() = default;
    virtual void update(float dt) = 0;
    virtual void render(Renderer &renderer) = 0;
};
