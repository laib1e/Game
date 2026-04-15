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
    if (not game) return;
    game->frame();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_game_GameActivity_onNativeOnTouch(JNIEnv *env, jobject thiz, jint action, jfloat y, jfloat x)
{
    game->onTouch(action, y, x);
}

extern "C"
JNIEXPORT jboolean JNICALL Java_com_example_game_GameActivity_onNativeShouldExit(JNIEnv *env, jobject thiz)
{
    if (game)
        return game->getShouldExit();
    return false;
}