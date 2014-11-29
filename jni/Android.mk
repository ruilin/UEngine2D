LOCAL_PATH := $(call my-dir)

MY_CFLAGS_OPTIMIZED 			:= -O3 -funsafe-loop-optimizations -fgcse-las -fgcse-sm		\
									-fgcse-after-reload
MY_CFLAGS 						:= $(MY_CFLAGS_OPTIMIZED) -Wall -Wpointer-arith 			\
									-Wdeclaration-after-statement -Wextra -Winline			\
									-Wdisabled-optimization -Wswitch-default				\
#									-Wwrite-strings	-Werror									\
#									-Wno-missing-field-initializers 
MY_EXTRA_CFLAGS 				:= -Wmissing-declarations -Wmissing-prototypes 				\
													-Wswitch-enum -Wfloat-equal								\
													-Wstrict-overflow=5
													
include $(CLEAR_VARS)

LOCAL_MODULE    				:= hello-jni
LOCAL_C_INCLUDES 		 		:= $(LOCAL_PATH)/engine 							\
									$(LOCAL_PATH)/engine/base	 					\
									$(LOCAL_PATH)/engine/base/base 					\
									$(LOCAL_PATH)/engine/base/engine 				\
									$(LOCAL_PATH)/engine/base/jni_main 				\
									$(LOCAL_PATH)/engine/java						\
									$(LOCAL_PATH)/engine/java/jni_lib				\
									$(LOCAL_PATH)/engine/render						\
									$(LOCAL_PATH)/engine/resource					\
									$(LOCAL_PATH)/engine/resource/res				\
									$(LOCAL_PATH)/engine/util						\
									$(LOCAL_PATH)/engine/util/data					\
									$(LOCAL_PATH)/engine/util/str_util				\
									$(LOCAL_PATH)/engine/util/anim					\
									$(LOCAL_PATH)/engine/io							\
									$(LOCAL_PATH)/engine/io/file_op					\
									$(LOCAL_PATH)/engine/runnable					\
									$(LOCAL_PATH)/engine/runnable/thread			\
									$(LOCAL_PATH)/engine/runnable/thread/gthread	\
									$(LOCAL_PATH)/engine/ui							\
									$(LOCAL_PATH)/engine/ui/ui						\
									$(LOCAL_PATH)/engine/ui/view_include			\
									$(LOCAL_PATH)/engine/render						\
									$(LOCAL_PATH)/engine/render/render2d			\
									$(LOCAL_PATH)/game								\
									$(LOCAL_PATH)/game/logic						\
									$(LOCAL_PATH)/game/logic/logic					\
									
################################################## ENGINE #####################################################											
LOCAL_SRC_FILES_ENGINE			:= engine/base/jni_main.c							\
									engine/base/engine.c							\
									engine/base/sysinfo.c							\
									engine/base/debug.c								\
									engine/java/jni_lib.c							\
									engine/util/time_util.c							\
									engine/util/str_util.c							\
									engine/util/mem.c								\
									engine/util/linked_list.c						\
									engine/util/hashmap/crc32.c						\
									engine/util/hashmap/hash.c						\
									engine/util/anim.c								\
									engine/util/data/endian.c						\
									engine/util/aes/aes256.c						\
									engine/util/data/rect.c							\
									engine/io/file_op.c								\
									engine/render/render3d/gl_renderer_3d.c			\
									engine/resource/image.c							\
									engine/resource/res.c							\
									engine/resource/png_reader.c					\
									engine/resource/pot.c							\
									engine/resource/map_data.c						\
									engine/resource/sprite_data.c					\
									engine/resource/script_data.c					\
									engine/render/render2d/renderer2d.c				\
									engine/render/render2d/graphic.c				\
									engine/render/render2d/canvas.c					\
									engine/render/render2d/text.c					\
									engine/runnable/runnable.c						\
									engine/runnable/thread/gthread.c				\
									engine/ui/ui.c									\
									engine/ui/view.c								\
									engine/ui/button.c								\
									engine/ui/motion_event.c						\
																
################################################### GAME #####################################################									
LOCAL_SRC_FILES 				:= 	$(LOCAL_SRC_FILES_ENGINE)						\
									$(LOCAL_SRC_FILES_ENGINE_EXT)					\
									game/logic/logic.c								\
									game/logic/view_debug.c							\
									game/render/renderer.c							\
#LOCAL_CFLAGS 			+= $(MY_CFLAGS) #$(MY_EXTRA_CFLAGS) -D _STAGES_LOG_TAG_='"U"'
LOCAL_LDLIBS   					:= -llog -landroid -lEGL -lGLESv1_CM -ljnigraphics -lz
LOCAL_WHOLE_STATIC_LIBRARIES 	:= png lua
include $(BUILD_SHARED_LIBRARY)

MY_LOCAL_PATH := $(LOCAL_PATH)
include $(LOCAL_PATH)/engine/ext/libpng/Android.mk
LOCAL_PATH := $(MY_LOCAL_PATH)
include $(LOCAL_PATH)/engine/ext/liblua/Android.mk

