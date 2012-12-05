LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sora_static

LOCAL_MODULE_FILENAME := libsora

# sora
LOCAL_SRC_FILES := ../tinyxml/tinyxmlparser.cpp  \
../tinyxml/tinystr.cpp  \
../tinyxml/tinyxml.cpp  \
../tinyxml/tinyxmlerror.cpp  \
{% for cpp_file in filelist %}../{{ cpp_file }}  \
{% endfor %}

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..    \
$(LOCAL_PATH)/../../cocos2dx    \
$(LOCAL_PATH)/../../cocos2dx/include    \
$(LOCAL_PATH)/../../cocos2dx/kazmath/include    \
$(LOCAL_PATH)/../../cocos2dx/platform    \
$(LOCAL_PATH)/../../cocos2dx/platform/android    \
$(LOCAL_PATH)/../../external/Box2D  \
$(LOCAL_PATH)/../../external  \
                                 
include $(BUILD_STATIC_LIBRARY)



