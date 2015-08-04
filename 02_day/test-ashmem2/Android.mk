LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= server.cpp
LOCAL_MODULE := my_server
LOCAL_SHARED_LIBRARIES:= libcutils libutils libbinder
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= client.cpp
LOCAL_MODULE := my_client
LOCAL_SHARED_LIBRARIES:= libcutils libutils libbinder
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)
