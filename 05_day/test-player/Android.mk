LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := test-player 
LOCAL_SRC_FILES := player.cpp
LOCAL_SHARED_LIBRARIES :=  libutils  libui libgui libmedia libbinder

include $(BUILD_EXECUTABLE)

