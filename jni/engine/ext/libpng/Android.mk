LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE			 := png
LOCAL_SRC_FILES_LIBPNG 	 := png.c 		\
							pngerror.c 	\
							pngget.c 	\
							pngmem.c 	\
							pngpread.c 	\
							pngread.c 	\
							pngrio.c 	\
							pngrtran.c 	\
							pngrutil.c 	\
							pngset.c 	\
							pngtrans.c 	\
							pngwio.c 	\
							pngwrite.c 	\
							pngwtran.c 	\
							pngwutil.c
LOCAL_SRC_FILES := $(LOCAL_SRC_FILES_LIBPNG)
LOCAL_CFLAGS += $(MY_CFLAGS)
include $(BUILD_STATIC_LIBRARY)
