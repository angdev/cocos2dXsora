LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sora_static

LOCAL_MODULE_FILENAME := libsora
#tinyxml
LOCAL_SRC_FILES += \
    ../tinyxml/tinystr.cpp  \
    ../tinyxml/tinyxml.cpp  \
    ../tinyxml/tinyxmlerror.cpp  \
    ../tinyxml/tinyxmlparser.cpp  \
    
# sora
LOCAL_SRC_FILES += \
    ../angle.cpp  \
    ../ani.cpp  \
    ../ani_data.cpp  \
    ../ani_frame.cpp  \
    ../ani_manager.cpp  \
    ../ani_node.cpp  \
    ../ani_parser.cpp  \
    ../ani_prototype.cpp  \
    ../ani_reader.cpp  \
    ../ani_writer.cpp  \
    ../class_attribute.cpp  \
    ../class_dict.cpp  \
    ../crc32.cpp  \
    ../data_define.cpp  \
    ../data_helper.cpp  \
    ../data_init.cpp  \
    ../logger.cpp  \
    ../math_helper.cpp  \
    ../mem.cpp  \
    ../string_hash.cpp  \
    ../string_util.cpp  \
    ../struct_version_converter.cpp  \
    ../vertex.cpp  \
    ../xml_node.cpp  \
    ../xml_writer.cpp  \
    ../xml_reader.cpp  \
    ../file_helper.cpp   \
    ../unit.cpp  \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..    \
    $(LOCAL_PATH)/../../cocos2dx    \
    $(LOCAL_PATH)/../../cocos2dx/include    \
    $(LOCAL_PATH)/../../cocos2dx/kazmath/include    \
    $(LOCAL_PATH)/../../cocos2dx/platform    \
    $(LOCAL_PATH)/../../cocos2dx/platform/android    \
    $(LOCAL_PATH)/../../external/Box2D  \
    $(LOCAL_PATH)/../../external  \
                                 
include $(BUILD_STATIC_LIBRARY)
