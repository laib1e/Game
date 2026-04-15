#pragma once
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

class Renderer
{
private:
    GLuint prog;
    float orthographicMatrix[16];
    int position_location = 0;
    int matrix_location = 0;
    int color_location = 0;

public:
    explicit Renderer(int width, int height);
    void drawRect(float x, float y, int w, int h, float r, float g, float b) const;
    void drawFrame();
};
