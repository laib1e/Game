#pragma once
#include <GLES2/gl2.h>

class Renderer
{
private:
    GLuint prog;
    float orthographicMatrix[16];
    int position_location = 0;
    int matrix_location = 0;
    int color_location = 0;

    GLuint fontProg = 0;
    GLuint fontTextureId = 0;
    int fontPosLoc = 0;
    int fontTexCoordLoc = 0;
    int fontProjLoc = 0;
    int fontColorLoc = 0;
    int fontCharCount = 0;
    static const int CHAR_SIZE = 8;
    const char* fontChars = "0123456789 SCOREGAMVITXD:";

    void initFont();
    int charIndex(char c) const;

public:
    explicit Renderer(int width, int height);
    void drawRect(float x, float y, int w, int h, float r, float g, float b) const;
    void drawFrame();
    void drawText(const char* text, float x, float y, float size, float r, float g, float b);
};
