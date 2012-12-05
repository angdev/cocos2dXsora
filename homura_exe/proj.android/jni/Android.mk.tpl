LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhomura

LOCAL_CPPFLAGS   +=  -fpermissive -Winvalid-offsetof


                   
# homura           
LOCAL_SRC_FILES += \
{% for cpp_file in filelist %}../../../homura/{{ cpp_file }}  \
{% endfor %}
                   
# homura_exe
LOCAL_SRC_FILES += ../../app_delegate.cpp \
                   hellocpp/main.cpp                                      

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../../    \
    $(LOCAL_PATH)/../../../    \
    $(LOCAL_PATH)/../../../sora    \
    $(LOCAL_PATH)/../../../homura    \
    $(LOCAL_PATH)/../../../external    \

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += sora_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,external/Box2D)
$(call import-module,sora/jni)
