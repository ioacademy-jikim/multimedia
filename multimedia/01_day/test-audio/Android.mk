LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog libutils  libmedia
LOCAL_SRC_FILES :=  my_audio_play.cpp
LOCAL_MODULE := my_audio_play
include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := liblog libutils  libmedia
LOCAL_SRC_FILES :=  my_audio_record.cpp
LOCAL_MODULE := my_audio_record
include $(BUILD_EXECUTABLE)



