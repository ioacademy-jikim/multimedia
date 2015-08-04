LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= thread.cpp
LOCAL_MODULE := my_thread
LOCAL_SHARED_LIBRARIES:= libcutils libutils libbinder
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)

