#include <cstring>
#include "Renderer.h"
#include <android/log.h>
#define LOG(...) __android_log_print(ANDROID_LOG_DEBUG, "Game", __VA_ARGS__)

static GLuint compile(GLenum paint, const GLchar* source)
{
    GLuint shader = glCreateShader(paint);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

static GLuint compile(const GLchar* glsl_vertex, const GLchar* glsl_fragment)
{
    GLuint vertex_shader = compile(GL_VERTEX_SHADER, glsl_vertex);
    GLuint fragment_shader = compile(GL_FRAGMENT_SHADER, glsl_fragment);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

static void createOrthoMatrix(float* matrix, float left, float right, float bottom, float top)
{
    float w = right - left;
    float h = top - bottom;

    // Столбец 1
    matrix[0] = 2.0f / w;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    // Столбец 2
    matrix[4] = 0.0f;
    matrix[5] = 2.0f / h;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    // Столбец 3
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = -1.0f;
    matrix[11] = 0.0f;

    // Столбец 4 (Трансляция)
    matrix[12] = -(right + left) / w;
    matrix[13] = -(top + bottom) / h;
    matrix[14] = 0;
    matrix[15] = 1.0f;
}

// 8x8 bitmap font glyphs — each character is 8 rows of 8 bits
static const unsigned char FONT_8X8[][8] = {
        // '0'
        {0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00},
        // '1'
        {0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00},
        // '2'
        {0x3C,0x66,0x06,0x0C,0x18,0x30,0x7E,0x00},
        // '3'
        {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00},
        // '4'
        {0x0C,0x1C,0x2C,0x4C,0x7E,0x0C,0x0C,0x00},
        // '5'
        {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00},
        // '6'
        {0x3C,0x60,0x7C,0x66,0x66,0x66,0x3C,0x00},
        // '7'
        {0x7E,0x06,0x0C,0x18,0x18,0x18,0x18,0x00},
        // '8'
        {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00},
        // '9'
        {0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00},
        // ' ' (space)
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        // 'S'
        {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00},
        // 'C'
        {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00},
        // 'O'
        {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
        // 'R'
        {0x7C,0x66,0x66,0x7C,0x6C,0x66,0x66,0x00},
        // 'E'
        {0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00},
        // 'G'
        {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3E,0x00},
        // 'A'
        {0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00},
        // 'M'
        {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00},
        // 'V'
        {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00},
        // 'I'
        {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00},
        // 'T'
        {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
        // 'X'
        {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00},
        // 'D'
        {0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00},
        // ':'
        {0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x00},
};

int Renderer::charIndex(char c) const
{
    for (int i = 0; fontChars[i] != '\0'; i++)
    {
        if (fontChars[i] == c) return i;
    }
    return -1;
}

void Renderer::initFont()
{
    // Count characters
    fontCharCount = 0;
    while (fontChars[fontCharCount] != '\0') fontCharCount++;

    // Create texture atlas: all chars in a row, each 8x8, RGBA
    int texW = CHAR_SIZE * fontCharCount;
    int texH = CHAR_SIZE;
    unsigned char* pixels = new unsigned char[texW * texH * 4];
    memset(pixels, 0, texW * texH * 4);

    for (int ci = 0; ci < fontCharCount; ci++)
    {
        for (int row = 0; row < 8; row++)
        {
            unsigned char bits = FONT_8X8[ci][row];
            for (int col = 0; col < 8; col++)
            {
                bool on = (bits >> (7 - col)) & 1;
                int px = ci * CHAR_SIZE + col;
                int py = row;
                int idx = (py * texW + px) * 4;
                pixels[idx + 0] = 255;  // R
                pixels[idx + 1] = 255;  // G
                pixels[idx + 2] = 255;  // B
                pixels[idx + 3] = on ? 255 : 0; // A
            }
        }
    }

    glGenTextures(1, &fontTextureId);
    glBindTexture(GL_TEXTURE_2D, fontTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    delete[] pixels;

    // Font shader
    const GLchar* vertSrc =
            "attribute vec2 aPos;"
            "attribute vec2 aUV;"
            "varying vec2 vUV;"
            "uniform mat4 uProj;"
            "void main() {"
            "  gl_Position = uProj * vec4(aPos, 0.0, 1.0);"
            "  vUV = aUV;"
            "}";

    const GLchar* fragSrc =
            "precision mediump float;"
            "varying vec2 vUV;"
            "uniform sampler2D uTex;"
            "uniform vec4 uColor;"
            "void main() {"
            "  float a = texture2D(uTex, vUV).a;"
            "  gl_FragColor = vec4(uColor.rgb, uColor.a * a);"
            "}";

    fontProg = compile(vertSrc, fragSrc);
    fontPosLoc = glGetAttribLocation(fontProg, "aPos");
    fontTexCoordLoc = glGetAttribLocation(fontProg, "aUV");
    fontProjLoc = glGetUniformLocation(fontProg, "uProj");
    fontColorLoc = glGetUniformLocation(fontProg, "uColor");
}

Renderer::Renderer(int width, int height)
{
    const GLchar* primitive_vertex
            =
            "attribute vec4 position;"
            "uniform mat4 projection;"
            "void main()"
            "{"
            "gl_Position = projection * position;"
            "}"
    ;

    const GLchar* primitive_fragment
            =
            "precision mediump float;"
            "uniform vec4 color;"
            "void main()"
            "{"
            "gl_FragColor = color;"
            "}"
    ;

    prog = compile(primitive_vertex, primitive_fragment);
    position_location = glGetAttribLocation(prog, "position");
    matrix_location = glGetUniformLocation(prog, "projection");
    color_location = glGetUniformLocation(prog, "color");

    createOrthoMatrix(orthographicMatrix, 0, (float)width, 0, (float)height);
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
    initFont();
}

void Renderer::drawRect(float x, float y, int w, int h, float r, float g, float b) const
{
    float vertices[] =
    {
            x,     y,
            x + (float)w, y,
            x,     y + (float)h,
            x + (float)w, y,
            x + (float)w, y + (float)h,
            x,     y + (float)h
    };

    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glUniform4f(color_location, r, g, b, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::drawFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(prog);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, orthographicMatrix);
}

void Renderer::drawText(const char* text, float x, float y, float size,
                        float r, float g, float b)
{
    glUseProgram(fontProg);
    glUniformMatrix4fv(fontProjLoc, 1, GL_FALSE, orthographicMatrix);
    glUniform4f(fontColorLoc, r, g, b, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTextureId);

    float cursorX = x;

    for (int i = 0; text[i] != '\0'; i++)
    {
        int idx = charIndex(text[i]);
        if (idx < 0)
        {
            cursorX += size * 0.6f;
            continue;
        }

        float u0 = (float)idx / (float)fontCharCount;
        float u1 = (float)(idx + 1) / (float)fontCharCount;

        float verts[] = {
                cursorX,        y,
                cursorX + size, y,
                cursorX,        y + size,
                cursorX + size, y,
                cursorX + size, y + size,
                cursorX,        y + size
        };

        float uvs[] = {
                u0, 1.0f,  u1, 1.0f,
                u0, 0.0f,  u1, 1.0f,
                u1, 0.0f,  u0, 0.0f
        };

        glEnableVertexAttribArray(fontPosLoc);
        glVertexAttribPointer(fontPosLoc, 2, GL_FLOAT, GL_FALSE, 0, verts);
        glEnableVertexAttribArray(fontTexCoordLoc);
        glVertexAttribPointer(fontTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, uvs);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        cursorX += size * 1.0f;
    }

    // Restore primitive shader
    glDisable(GL_BLEND);
    glUseProgram(prog);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, orthographicMatrix);
}