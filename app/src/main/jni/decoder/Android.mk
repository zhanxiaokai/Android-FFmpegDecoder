LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm 
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../3rdparty/ffmpeg/include
     
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3 

LOCAL_SRC_FILES += \
./libffmpeg_decoder/accompany_decoder_controller.cpp \
 ./libffmpeg_decoder/accompany_decoder.cpp 
    
LOCAL_MODULE := libffmpeg_decoder
include $(BUILD_STATIC_LIBRARY)