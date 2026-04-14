package com.example.game;

import android.annotation.SuppressLint;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

import androidx.appcompat.app.AppCompatActivity;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GameActivity extends AppCompatActivity
{
    static
    {
        System.loadLibrary("game");
    }

    @Override
    public void onCreate(Bundle savedState)
    {
        super.onCreate(savedState);
        NativeSurface nativeSurface = new NativeSurface(this);
        setContentView(nativeSurface);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
    }

    private class NativeSurface extends GLSurfaceView
    {
        public NativeSurface(Context context)
        {
            super(context);
            setEGLContextClientVersion(2);
            setPreserveEGLContextOnPause(true);

            setRenderer(new NativeRender());
            setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        }

        @SuppressLint("ClickableViewAccessibility")
        @Override
        public boolean onTouchEvent(MotionEvent event)
        {
            onNativeOnTouch(event.getAction(), event.getY());
            return true;
        }
    }

    private class NativeRender implements GLSurfaceView.Renderer
    {
        @Override
        public void onDrawFrame(GL10 gl)
        {
            onNativeDrawFrame();
        }

        @Override
        public void onSurfaceChanged(GL10 gl10, int width, int height)
        {
            onNativeResize(width, height);
        }

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config)
        {
            onNativeSurfaceCreate();
        }
    }

    native void onNativeSurfaceCreate();
    native void onNativeResize(int width, int height);
    native void onNativeDrawFrame();
    native void onNativeOnTouch(int action, float y);
}
