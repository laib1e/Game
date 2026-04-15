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
    bool collides(const GameObject& other) const
    {
        return X < other.X + (float)other.Width and
               X + (float)Width > other.X and
               Y < other.Y + (float)other.Height and
               Y + (float)Height > other.Y;
    };
    void setActive(bool set)
    {
        Active = set;
    }
    bool isActive() const
    {
        return Active;
    }
    void setX(float x)
    {
        X = x;
    }
    void setY(float y)
    {
        Y = y;
    }
    float getX() const
    {
        return X;
    }
    float getY() const
    {
        return Y;
    }
    int getWidth() const
    {
        return Width;
    }
    int getHeight() const
    {
        return Height;
    }
};
