/*
 * engine.c
 *
 *  Created on: 2013-9-3
 *      Author: ryan
 */

#include "engine.h"

PRIVATE Engine engine;

PUBLIC Engine *engine_init(JNIEnv * env) {
	engine.gametime = 0;
	engine.env = env;
	engine.state = 0;
	engine.screenWidth = 800;
	engine.screenHeight = 480;
	engine.canvasWidth = 800;
	engine.canvasHeight = 480;
	engine.data = NULL;
	engine.linked_event = NULL;
	engine.g = NULL;
	engine.isLoading = FALSE;
	engine.loadingRatio = 0.0f;
	engine.maxRatio = 0.0f;
	engine.debug = debug_init();
	jni_lib_init(engine.env);
	engine.sysinfo = sysinfo_init();
	file_op_init(engine.sysinfo->externalStorageDirectoryName);

	return &engine;
}

PUBLIC void engine_end(Engine *engine) {
	sysinfo_destroy(); engine->sysinfo = NULL;
	jni_lib_end();
	return;
}

PUBLIC Engine *engine_get() {
	return &engine;
}

PUBLIC BOOL engine_setLoading(BOOL yes, float maxRatio) {
	engine.isLoading = yes;
	engine.maxRatio = maxRatio;
	return TRUE;
}

PUBLIC void engine_setLoadingRatio(float ratio) {
	engine.loadingRatio += ratio;
	if (engine.loadingRatio > engine.maxRatio) {
		engine.loadingRatio = engine.maxRatio;
	}
	return;
}

PUBLIC void engine_startTransEffect(ENGINE_CALLBACK callback_effect_end, void *param) {
	engine.ifDrawTransform = TRUE;
	engine.callback_effect_end = callback_effect_end;
	engine.callback_param = param;
	return;
}

PUBLIC void engine_finishTransEffect() {
	engine.ifDrawTransform = FALSE;
	if (NULL != engine.callback_effect_end) {
		engine.callback_effect_end(engine.callback_param);
	}
	return;
}
