LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := test-openmax
LOCAL_MODULE_TAGS := tests

LOCAL_C_INCLUDES := \
			$(LOCAL_PATH)/include \
			bionic/ \
 			external/stlport/stlport \
 			frameworks/base/include \
 			frameworks/av/media/libstagefright \
			frameworks/native/include/media/openmax \



LOCAL_SRC_FILES := \
				AnatomyOMXCore.cpp \
				AnatomyOMXPlugin.cpp \
				main_anatomy_omxclient.cpp \
				AnatomyOMXClient.cpp \
				ATMParser.cpp \
				StdoutRenderer.cpp \

LOCAL_SHARED_LIBRARIES := \
		libcutils\
		libutils\
		libdl\
 		libstlport \
 		libstagefright_omx \
 		libstagefright_foundation \

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := libanatomy_atmdec
LOCAL_MODULE_TAGS := tests

LOCAL_C_INCLUDES := \
			$(LOCAL_PATH)/include \
			bionic/ \
 			external/stlport/stlport \
 			frameworks/base/include \
 			frameworks/av/media/libstagefright \
			frameworks/native/include/media/openmax \

LOCAL_SHARED_LIBRARIES := \
		libcutils\
		libutils\
		libdl\
 		libstlport \
 		libstagefright_omx \
 		libstagefright_foundation \

LOCAL_SRC_FILES := \
				AnatomyOMXComponent.cpp \

include $(BUILD_SHARED_LIBRARY)
