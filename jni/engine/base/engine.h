/*
 * engine.h
 *
 *  Created on: 2013-9-3
 *      Author: ryan
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <stdlib.h>
#include <GLES/gl.h>
#include <base.h>
#include <jni.h>
#include <resource/image.h>
#include <ui/motion_event.h>
#include <ui/ui.h>
#include "debug.h"
#include "sysinfo.h"


typedef BOOL (*ENGINE_CALLBACK) (void *param);

typedef struct {
	unsigned long long gametime; /* ms */
	Debug *debug;
	JNIEnv *env;
	struct Sysinfo *sysinfo;
    int32_t state;
    int32_t screenWidth;
    int32_t screenHeight;
    unsigned short canvasWidth;
    unsigned short canvasHeight;
    void *data;
    void *g;
    void *linked_event;	/* linked list of motion events from java */
    /* loading state */
    BOOL isLoading;
    float loadingRatio;
    float maxRatio;
    /* view transform effect */
    ENGINE_CALLBACK callback_effect_end;
    void *callback_param;
    BOOL ifDrawTransform;
} Engine;

Engine *engine_init(JNIEnv * env);
void engine_end(Engine *engine);
Engine *engine_get();
BOOL engine_setLoading(BOOL yes, float maxRatio);
void engine_setLoadingRatio(float ratio);
void engine_startTransEffect(ENGINE_CALLBACK callback_effect_end, void *param);
void engine_finishTransEffect();

#endif /* ENGINE_H_ */
