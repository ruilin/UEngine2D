/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#include "jni_lib.h"
/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/ugame/main/Uactivity.java
 */

PRIVATE struct JniLib {
	JNIEnv *env;
	AAssetManager *assetManager;
	jclass javaLibCls;
	jclass langStringClass; jobject langStringClassRef;
} Jni_lib;

PUBLIC JNIEnv *jni_lib_getEnv() {
	return Jni_lib.env;
}

PRIVATE jobject _jni_lib_getJavaAssetManager(JNIEnv* env) {
	jclass cls = (*env)->FindClass(env,"com/ugame/main/Uactivity");
	if (NULL == cls) {
		LOGE("_jni_lib_getJavaAssetManager() NULL == cls");
		return NULL;
	}
	/* call static method */
	jmethodID mId = (*env)->GetStaticMethodID(env, cls, "getAssetManager", "()Landroid/content/res/AssetManager;");
	return (*env)->CallStaticObjectMethod(env, cls, mId);
}

PUBLIC void jni_lib_init_langStringClass() {
	JNIEnv *env;
	env = Jni_lib.env;
	Jni_lib.langStringClass = (*env)->FindClass(env, "java/lang/String");
	/* creates a global reference on the class and hangs on to it */
	Jni_lib.langStringClassRef = (*env)->NewGlobalRef(env, Jni_lib.langStringClass);
	return;
}

PUBLIC void jni_lib_init(JNIEnv* env) {
	Jni_lib.env = env;
	jobject assetFormJava = _jni_lib_getJavaAssetManager(env);
	Jni_lib.assetManager = AAssetManager_fromJava(Jni_lib.env, assetFormJava);
	if(Jni_lib.assetManager == NULL) {
	  LOGE("jni_lib_init() AAssetManager==NULL");
	  return;
	}
	Jni_lib.javaLibCls = (*Jni_lib.env)->FindClass(Jni_lib.env,"com/ugame/help/Helper");
	if (NULL == Jni_lib.javaLibCls) {
		LOGE("jni_lib_init() NULL == javaLibCls");
		return;
	}
	jni_lib_init_langStringClass();
	return;
}

PUBLIC void jni_lib_end() {

	return;
}

PUBLIC AAsset *jni_lib_getAAsset(const char *filename, off_t *size) {
	AAsset *asset = AAssetManager_open(Jni_lib.assetManager, filename, AASSET_MODE_UNKNOWN);
	if(asset == NULL) {
	  LOGE("jni_lib_getAAsset() asset == NULL, filename: %s", filename);
	  return NULL;
	}
	*size = AAsset_getLength(asset);
	return asset;
}

jstring
Java_com_ugame_main_Uactivity_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
	jclass cls = (*env)->FindClass(env,"com/ugame/main/Uactivity");
	if (NULL != cls) {
		/* call static method */
		jmethodID mId = (*env)->GetStaticMethodID(env, cls, "log", "()V");
		(*env)->CallStaticVoidMethod(env, cls, mId);

		/* call method */
		jmethodID mId2 = (*env)->GetMethodID(env, cls, "log2", "()V");
		jmethodID construction_id = (*env)->GetMethodID(env, cls, "<init>", "()V");
		jobject mTestProvider = (*env)->NewObject(env, cls, construction_id);
		(*env)->CallVoidMethod(env, mTestProvider, mId2);
	}
    return (*env)->NewStringUTF(env, "Hello from JNI !");
}

PUBLIC unsigned char *jni_lib_readFromAssets(const char *filename, unsigned int *bufferSize) {
	/*获取文件名并打开*/
	AAsset *asset = AAssetManager_open(Jni_lib.assetManager, filename, AASSET_MODE_UNKNOWN);
	if(asset == NULL) {
	  LOGE("jni_lib_readFromAssets() asset == NULL, filename: %s", filename);
	  return NULL;
	}
	/*获取文件大小*/
	off_t _bufferSize = AAsset_getLength(asset);
	char *buffer = (char *)MALLOC(_bufferSize + 1);
	buffer[_bufferSize] = 0;
	int numBytesRead = AAsset_read(asset, buffer, _bufferSize);
	if (NULL != bufferSize) *bufferSize = _bufferSize;
	/*关闭文件*/
	AAsset_close(asset);
	return buffer;
}

/*
 * 复制所有文件到SD卡（不包括子目录）
 */
PUBLIC void jni_lib_cpy_allFileToSdcard(const char *path) {
	/* TODO 暂时去掉判断，方便调试和更新 */
//	if (FALSE == file_op_isExists(path)) {
		AAssetDir *dir = AAssetManager_openDir(Jni_lib.assetManager, path);
		if(dir == NULL) {
			LOGE("jni_lib_iterator_allFile() asset == NULL, filename: %s", path);
			return;
		}
		file_op_mkdir(path);
		const char *filename;
		unsigned char filepath[128];
		while (NULL != (filename = AAssetDir_getNextFileName(dir))) {
			int size;
			sprintf(filepath, "%s/%s", path, filename);
			unsigned char *filecontent = jni_lib_readFromAssets(filepath, &size);
			file_op_writeFile(filepath, filecontent, size);
			FREE(filecontent);
		}
		AAssetDir_close(dir);
//	}
	return;
}

/*
 * 读取文件数据传递给java转换为bitmap再传回来
 * （比较耗时）
 */
PUBLIC unsigned char *jni_lib_readPngFromAssets(const char *filename, jobject *bitmap) {
	unsigned int bufferSize;
	unsigned char *buffer = jni_lib_readFromAssets(filename, &bufferSize);
	jmethodID mId = (*Jni_lib.env)->GetStaticMethodID(Jni_lib.env, Jni_lib.javaLibCls, "convertToBMP", "([B)Landroid/graphics/Bitmap;");

	jbyteArray jbyteArr = (*Jni_lib.env)->NewByteArray(Jni_lib.env, bufferSize);
	(*Jni_lib.env)->SetByteArrayRegion(Jni_lib.env, jbyteArr, 0, bufferSize, buffer);
	FREE(buffer);

	*bitmap = ((*Jni_lib.env)->CallStaticObjectMethod(Jni_lib.env, Jni_lib.javaLibCls, mId, jbyteArr));
	(*Jni_lib.env)->DeleteLocalRef(Jni_lib.env, jbyteArr);
	return NULL;
}

/*
 * 获取由java读取的bitmap
 */
PUBLIC BOOL jni_lib_loadJavaAssetBitmap(const char *filename, jobject *bitmap) {
	/* call static method */
	jstring str = (*Jni_lib.env)->NewStringUTF(Jni_lib.env, filename);
	jmethodID mId = (*Jni_lib.env)->GetStaticMethodID(Jni_lib.env, Jni_lib.javaLibCls, "getAssetBitmap", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	*bitmap = ((*Jni_lib.env)->CallStaticObjectMethod(Jni_lib.env, Jni_lib.javaLibCls, mId, str));
	(*Jni_lib.env)->DeleteLocalRef(Jni_lib.env, str);
	return TRUE;
}

/****************************************************************************************************************************/
/* text bitmap */

PUBLIC BOOL jni_lib_textToBitmapPOT(const char *strToDraw, int fontsize,
									int strokeWidth, int paintStyle,
									jobject *bitmapObject,
									int *bitmapWidth, int *bitmapHeight,
									int *bitmapWidthPOT, int *bitmapHeightPOT,
									int *textPixelSize,
									int r, int g, int b, int a,
									int strokeR, int strokeG, int strokeB, int strokeA) {
	JNIEnv *env; jclass cls; jmethodID m; jstring jstr; jintArray iArray;
	int *retArray;
	env = Jni_lib.env;
	cls = Jni_lib.javaLibCls;
	if (NULL == cls) {
		LOGE("jni_lib_textToBitmapPOT() NULL == cls");
		return FALSE;
	}
	m = (*env)->GetStaticMethodID(env, cls, "textToBitmapPOT", "(Ljava/lang/String;[IIIIIIIIIIII)Landroid/graphics/Bitmap;");
	if (NULL == m) {
		LOGE("jni_lib_textToBitmapPOT(): GetStaticMethodID() failed");
		return FALSE;
	}
	jstr = (*env)->NewStringUTF(env, strToDraw);
	iArray = (*env)->NewIntArray(env, 5);
	*bitmapObject = (*env)->CallStaticObjectMethod(env, cls, m, jstr, iArray, fontsize, strokeWidth, paintStyle, r, g, b, a, strokeR, strokeG, strokeB, strokeA);
	retArray = (*env)->GetIntArrayElements(env, iArray, JNI_FALSE);
	*bitmapWidth = retArray[0];
	*bitmapHeight = retArray[1];
	*bitmapWidthPOT = retArray[2];
	*bitmapHeightPOT = retArray[3];
	*textPixelSize = retArray[4];
	(*env)->ReleaseIntArrayElements(env, iArray, retArray, JNI_ABORT);
	(*env)->DeleteLocalRef(env, iArray);
	(*env)->DeleteLocalRef(env, jstr);
	return TRUE;
}

PUBLIC void jni_lib_bitmapToPixels(jobject bitmap, unsigned char **pixels) {
	int ret;
	if ((ret = AndroidBitmap_lockPixels(Jni_lib.env, bitmap, (void **)(pixels))) < 0) {
		LOGE("jni_lib_bitmapToPixels() error: %d", ret);
	}
	AndroidBitmap_unlockPixels(Jni_lib.env, bitmap);
	return;
}

PUBLIC BOOL jni_lib_helper_getSysInfo(char **stringArray, int arrayCount, long long *longArray, int longArrayCount) {
	struct _jlib *j; JNIEnv *env; jclass c; jmethodID m; jstring jstr; jobjectArray objArray; int i; jlongArray javaLongArray; jlong *javaLong;
	env = Jni_lib.env;
	c = Jni_lib.javaLibCls;
	m = (*env)->GetStaticMethodID(env, c, "getSysInfo", "([Ljava/lang/String;[J)V");
	if (NULL == m) {
		LOGE("jni_lib_helper_getSysInfo(): GetStaticMethodID() failed");
		return FALSE;
	}
	objArray = (*env)->NewObjectArray(env, arrayCount, Jni_lib.langStringClass, NULL);
	javaLongArray = (*env)->NewLongArray(env, longArrayCount);
	(*env)->CallStaticVoidMethod(env, c, m, objArray, javaLongArray);
	for (i = 0; i < arrayCount; i++) {
		const char *nativeString;
		jstr = (*env)->GetObjectArrayElement(env, objArray, i);
		if (NULL != jstr) {
			stringArray[i] = (char*)MALLOC((*env)->GetStringUTFLength(env, jstr) + 1);
			nativeString = (*env)->GetStringUTFChars(env, jstr, 0);
			strcpy(stringArray[i], nativeString);
			(*env)->ReleaseStringUTFChars(env, jstr, nativeString);
		} else {
			stringArray[i] = (char*)MALLOC(4 + 1);
			strcpy(stringArray[i], "null");
		}
		(*env)->DeleteLocalRef(env, jstr);
	}
	javaLong = (*env)->GetLongArrayElements(env, javaLongArray, 0);
	memcpy((void *)longArray, (void *)javaLong, sizeof(jlong) * longArrayCount);
	(*env)->ReleaseLongArrayElements(env, javaLongArray, javaLong, JNI_ABORT);
	(*env)->DeleteLocalRef(env, javaLongArray);
	(*env)->DeleteLocalRef(env, objArray);
	return TRUE;
}
