LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhomura

LOCAL_CPPFLAGS   +=  -fpermissive -Winvalid-offsetof

#tinyxml
LOCAL_SRC_FILES += \
    ../../../sora/tinyxml/tinystr.cpp  \
    ../../../sora/tinyxml/tinyxml.cpp  \
    ../../../sora/tinyxml/tinyxmlerror.cpp  \
    ../../../sora/tinyxml/tinyxmlparser.cpp  \
    
# sora
LOCAL_SRC_FILES += \
    ../../../sora/angle.cpp  \
    ../../../sora/ani.cpp  \
    ../../../sora/ani_data.cpp  \
    ../../../sora/ani_frame.cpp  \
    ../../../sora/ani_manager.cpp  \
    ../../../sora/ani_node.cpp  \
    ../../../sora/ani_parser.cpp  \
    ../../../sora/ani_prototype.cpp  \
    ../../../sora/ani_reader.cpp  \
    ../../../sora/ani_writer.cpp  \
    ../../../sora/class_attribute.cpp  \
    ../../../sora/class_dict.cpp  \
    ../../../sora/crc32.cpp  \
    ../../../sora/data_define.cpp  \
    ../../../sora/data_helper.cpp  \
    ../../../sora/data_init.cpp  \
    ../../../sora/logger.cpp  \
    ../../../sora/math_helper.cpp  \
    ../../../sora/mem.cpp  \
    ../../../sora/string_hash.cpp  \
    ../../../sora/string_util.cpp  \
    ../../../sora/struct_version_converter.cpp  \
    ../../../sora/vertex.cpp  \
    ../../../sora/xml_node.cpp  \
    ../../../sora/xml_writer.cpp  \
    ../../../sora/xml_reader.cpp  \
    ../../../sora/file_helper.cpp
                   
# homura           
LOCAL_SRC_FILES += \
    ../../../homura/game_component.cpp  \
    ../../../homura/game_console.cpp  \
    ../../../homura/game_message.cpp  \
    ../../../homura/game_object.cpp  \
    ../../../homura/game_world.cpp  \
    ../../../homura/hello_world_scene.cpp  
                   
# homura_exe
LOCAL_SRC_FILES += ../../app_delegate.cpp \
                   hellocpp/main.cpp                                      

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../../    \
    $(LOCAL_PATH)/../../../    \
    $(LOCAL_PATH)/../../../sora    \
    $(LOCAL_PATH)/../../../homura    \

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)

