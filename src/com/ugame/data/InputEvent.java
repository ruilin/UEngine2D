package com.ugame.data;

public class InputEvent {

	public int[][] pointers;
	
	public InputEvent() {
		
	}
	
	public static InputEvent getInstance(int pointerCount) {
		InputEvent event = new InputEvent();
		event.pointers = new int[pointerCount][3];	/* 3 int each pointer(action, x, y) */
		return event;
	}

}
