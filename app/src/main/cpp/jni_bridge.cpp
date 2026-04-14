#include <jni.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeSurfaceCreate(JNIEnv *env, jobject thiz)
{
    glClearColor(0.2, 0.3, 0.3, 0.1);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeResize(JNIEnv *env, jobject thiz, jint width, jint height)
{
    glViewport(0, 0, width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeDrawFrame(JNIEnv *env, jobject thiz)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeOnTouch(JNIEnv *env, jobject thiz, jint action, jfloat y)
{
    // TODO: implement onNativeOnTouch()
}