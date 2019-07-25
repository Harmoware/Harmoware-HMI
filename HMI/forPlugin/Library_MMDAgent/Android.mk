LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := MMDAgent
LOCAL_SRC_FILES  := src/lib/BoneController.cpp \
                    src/lib/LipSync.cpp \
                    src/lib/LogText.cpp \
                    src/lib/Message.cpp \
                    src/lib/MMDAgent.cpp \
                    src/lib/MMDAgent_utils.cpp \
                    src/lib/MotionStocker.cpp \
                    src/lib/Option.cpp \
                    src/lib/PMDObject.cpp \
                    src/lib/Plugin.cpp \
                    src/lib/Render.cpp \
                    src/lib/ScreenWindow.cpp \
                    src/lib/Stage.cpp \
                    src/lib/FreeTypeGL.cpp \
                    src/lib/TileTexture.cpp \
                    src/lib/Timer.cpp \
                    src/lib/Menu.cpp \
                    src/lib/FileBrowser.cpp \
                    src/lib/Prompt.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/../Library_JPEG/include \
                    $(LOCAL_PATH)/../Library_Bullet_Physics/include \
                    $(LOCAL_PATH)/../Library_GLee/include \
                    $(LOCAL_PATH)/../Library_libpng/include \
                    $(LOCAL_PATH)/../Library_zlib/include \
                    $(LOCAL_PATH)/../Library_MMDFiles/include \
                    $(LOCAL_PATH)/../Library_GLFW/include \
                    $(LOCAL_PATH)/../Library_FreeType/include \
                    $(LOCAL_PATH)/../Library_UTF8-CPP/include
LOCAL_CFLAGS     += -DMMDAGENT_DONTRENDERDEBUG \
                    -DMMDAGENT_DONTUSESHADOWMAP \
                    -DMMDAGENT_DONTPICKMODEL \
                    -DMMDAGENT_DONTUSEMOUSE \
                    -DMMDAGENT_DONTUSEWINDOW \
                    -DMMDAGENT \
                    -DMMDAGENT_OVERWRITEEXEFILE="\"/sdcard/MMDAgent/MMDAgent.exe\"" \
                    -DMMDAGENT_OVERWRITECONFIGFILE="\"/sdcard/MMDAgent_Example/MMDAgent_Example.mdf\"" \
                    -DMMDAGENT_OVERWRITESYSDATADIR="\"/sdcard/MMDAgent/AppData\"" \
                    -DMMDAGENT_OVERWRITEPLUGINDIR="\"/data/data/jp.mmdagent/lib\""

include $(BUILD_STATIC_LIBRARY)
