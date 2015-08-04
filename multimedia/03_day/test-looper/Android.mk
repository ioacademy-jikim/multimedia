LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog libutils  libmedia
LOCAL_SRC_FILES :=  my_looper.cpp
LOCAL_MODULE := my_looper
include $(BUILD_EXECUTABLE)


