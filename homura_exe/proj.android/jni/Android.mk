LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhomura

LOCAL_CPPFLAGS   +=  -fpermissive -Winvalid-offsetof


                   
# homura           
LOCAL_SRC_FILES += \
    ../../../homura/game_component.cpp  \
    ../../../homura/game_console.cpp  \
    ../../../homura/game_message.cpp  \
    ../../../homura/game_object.cpp  \
    ../../../homura/game_world.cpp  \
    ../../../homura/game_object_factory.cpp \
    ../../../homura/drawable_component.cpp  \
    ../../../homura/main_menu_layer.cpp \
    ../../../homura/option_layer.cpp    \
    ../../../homura/game_layer.cpp  \
    ../../../homura/logic_component.cpp \
    ../../../homura/collision_tuple.cpp \
    ../../../homura/GLES-Render.cpp \
    ../../../homura/phy_component.cpp   \
    ../../../homura/phy_world.cpp   \
    ../../../homura/ai_character_component.cpp   \
    ../../../homura/bullet_component.cpp   \
    ../../../homura/character_component.cpp   \
    ../../../homura/combat_plane_component.cpp   \
    ../../../homura/debug_config.cpp   \
    ../../../homura/game_stage_factory.cpp   \
    ../../../homura/player_component.cpp   \
    ../../../homura/game_stage.cpp  \
    
                   
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
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += sora_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,external/Box2D)
$(call import-module,sora/jni)
