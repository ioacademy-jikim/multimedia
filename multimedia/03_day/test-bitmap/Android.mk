LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=  test-bitmap.cpp

LOCAL_SHARED_LIBRARIES := libutils  libgui libui

LOCAL_MODULE:= test-bitmap
LOCAL_CFLAGS:= -g -O0

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)

