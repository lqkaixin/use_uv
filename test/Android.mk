LOCAL_PATH  := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE  := test

LOCAL_C_INCLUDES  += \
  $(LOCAL_PATH)/../common/include \
  $(TOPDIR)system/core/include/

LOCAL_SRC_FILES += \
  test.c
  
  
LOCAL_CFLAGS += -MD -DLINUX -D_ANDROID_LINUX -DMOBILE_PHONE -D_VOD_MEM_20M -DENABLE_KANKAN_CDN -D_CONNECT_DETAIL -O0 -D_LOGGER

LOCAL_STATIC_LIBRARIES += libcommon libxlxml

LOCAL_LDLIBS  += -lz -llog

include $(BUILD_EXECUTABLE)
#include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
