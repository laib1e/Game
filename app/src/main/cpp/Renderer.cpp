#include "Renderer.h"

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

Renderer::Renderer(int width, int height)
{
    const GLchar* glsl_vertex
            =
            "attribute vec4 position;"
            "uniform mat4 projection;"
            "void main()"
            "{"
            "gl_Position = projection * position;"
            "}"
    ;

    const GLchar* glsl_fragment
            =
            "precision mediump float;"
            "uniform vec4 color;"
            "void main()"
            "{"
            "gl_FragColor = color;"
            "}"
    ;
    prog = compile(glsl_vertex, glsl_fragment);
    position_location = glGetAttribLocation(prog, "position");
    matrix_location = glGetUniformLocation(prog, "projection");
    color_location = glGetUniformLocation(prog, "color");

    createOrthoMatrix(orthographicMatrix, 0, (float)width, 0, (float)height);
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
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
