package com.ugame.main;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import com.ugame.data.InputEvent;

public class GlGameView extends GLSurfaceView {
	GlRenderer renderer;
	
	public GlGameView(Context context) {
		super(context);
		setFocusable(true);
		requestFocus();
		
		renderer = new GlRenderer();
		setRenderer(renderer);
		
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (event.getPointerCount() > 0) {
			InputEvent inputEvent = InputEvent.getInstance(event.getPointerCount());
			for (int i = 0; i < event.getPointerCount(); i++) {
				inputEvent.pointers[i][0] = event.getAction();
				inputEvent.pointers[i][1] = (int) event.getX(i);
				inputEvent.pointers[i][2] = (int) event.getY(i);
			}
			renderer.addEvent(inputEvent);
			return true;
		}
		return false;
	}
	
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		renderer.createWorld();
		super.surfaceCreated(holder);
	}
	
	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		renderer.destroyWorld();
		super.surfaceDestroyed(holder);
	}

}
