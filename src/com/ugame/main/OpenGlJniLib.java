package com.ugame.main;

import com.ugame.data.Platform;

public class OpenGlJniLib {
    static {
        System.loadLibrary("hello-jni");
    }

    public static native Platform init(Platform globalData, int width, int height);
    public static native void createWorld();
    public static native void create();
    public static native Platform step(Platform globalData);
    public static native void destroy(Platform globalData);
    public static native Platform getGlobalData(Platform globalData);
}
