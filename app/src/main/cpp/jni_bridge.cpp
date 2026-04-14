#include <jni.h>
#include <chrono>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

static GLuint compile(GLenum paint, const GLchar* source)
{
    GLuint shader = glCreateShader(paint);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

static GLuint compile(const GLchar* glsl_vertex, const GLchar* glsl_fragment)
{
    auto vertex_shader = compile(GL_VERTEX_SHADER, glsl_vertex);
    auto fragment_shader = compile(GL_FRAGMENT_SHADER, glsl_fragment);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glUseProgram(program);

    return program;
}

void createOrthoMatrix(float* matrix, float left, float right, float bottom, float top) {
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
std::chrono::steady_clock::time_point timestamp;
float orthographicMatrix[16];
float dt = 0;
GLuint prog;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeSurfaceCreate(JNIEnv *env, jobject thiz)
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

    timestamp = std::chrono::steady_clock::now();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeResize(JNIEnv *env, jobject thiz, jint width, jint height)
{
    glViewport(0, 0, width, height);
    createOrthoMatrix(orthographicMatrix, 0, width, 0, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeDrawFrame(JNIEnv *env, jobject thiz)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2, 0.3, 0.3, 0.1);

    static float posY = 300.0f;
    posY += 200.0f * dt;
    auto timeframe = std::chrono::steady_clock::now();
    dt = static_cast<decltype(dt)>((timeframe - timestamp).count()) / std::chrono::steady_clock::period::den;
    timestamp = timeframe;

    int matrix_location = glGetUniformLocation(prog, "projection");
    int color_location = glGetUniformLocation(prog, "color");
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, orthographicMatrix);
    glUniform4f(color_location, 1.0f, 0.0f, 0.0f, 1.0f);

    float x = 100, y = posY, w = 60, h = 40;
    float vertices[] = {
            x,     y,
            x + w, y,
            x,     y + h,
            x + w, y,
            x + w, y + h,
            x,     y + h
    };

    int posLoc = glGetAttribLocation(prog, "position");
    glEnableVertexAttribArray(posLoc);
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 0, vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeOnTouch(JNIEnv *env, jobject thiz, jint action, jfloat y)
{
    // TODO: implement onNativeOnTouch()
}