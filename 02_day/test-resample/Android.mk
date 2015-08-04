LOCAL_PATH:= $(call my-dir)


include $(CLEAR_VARS)

LOCAL_SRC_FILES:=               \
    test-resample.cpp           \

LOCAL_C_INCLUDES := \
    $(call include-path-for, audio-utils)

LOCAL_STATIC_LIBRARIES := \
    libsndfile

LOCAL_SHARED_LIBRARIES := \
    libaudioresampler \
    libaudioutils \
    libdl \
    libcutils \
    libutils \
    liblog

LOCAL_MODULE:= test-resample

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
