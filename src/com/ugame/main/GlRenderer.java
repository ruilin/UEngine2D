package com.ugame.main;

import java.util.ArrayList;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.ugame.data.InputEvent;
import com.ugame.data.Platform;

import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;

public class GlRenderer implements Renderer {
	private final static InputEvent[] INPUT_EVENT_TYPE = new InputEvent[0];
	private ArrayList<InputEvent> events = new ArrayList<InputEvent>();
	
	public Platform globalData;
	
	public GlRenderer() {
		globalData = new Platform();
	}
	
	public void onDrawFrame(GL10 gl) {
		if (0 < events.size()) {
			globalData.inputs = new InputEvent[events.size()];
			globalData.inputs = events.toArray(INPUT_EVENT_TYPE);
			events.clear();
		} else {
			globalData.inputs = null;
		}
		OpenGlJniLib.step(globalData);
	}
	
	public void addEvent(InputEvent pointers) {
		events.add(pointers);
	}
	
	public void createWorld() {
		OpenGlJniLib.createWorld();
	}
	
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		long st = System.currentTimeMillis();
		OpenGlJniLib.init(globalData, width, height);
		Log.e(null, "time init: " + (System.currentTimeMillis() - st));
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        OpenGlJniLib.create();
	}
	
	public void destroyWorld() {
		OpenGlJniLib.destroy(globalData);
	}
	
}
