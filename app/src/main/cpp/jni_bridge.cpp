#include <jni.h>
#include "Game.h"

Game *game = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeSurfaceCreate(JNIEnv *env, jobject thiz)
{

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeResize(JNIEnv *env, jobject thiz, jint width, jint height)
{
    game = new Game(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeDrawFrame(JNIEnv *env, jobject thiz)
{
    game->frame();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeOnTouch(JNIEnv *env, jobject thiz, jint action, jfloat y)
{
    game->onTouch(action, y);
}