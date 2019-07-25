
TARGET   = lib/MMDAgent.a

SOURCES  = src/lib/BoneController.cpp \
           src/lib/FreeTypeGL.cpp \
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
           src/lib/TileTexture.cpp \
           src/lib/Timer.cpp \
           src/lib/Menu.cpp \
           src/lib/FileBrowser.cpp \
           src/lib/Prompt.cpp

OBJECTS  = $(SOURCES:.cpp=.o)

CXX      = gcc
AR       = ar
CXXFLAGS = -Wall -O3 -fomit-frame-pointer -fPIC \
           -DMMDAGENT
INCLUDE  = -I ../Library_Bullet_Physics/include \
           -I ../Library_GLee/include \
           -I ../Library_zlib/include \
           -I ../Library_libpng/include \
           -I ../Library_FreeType/include \
           -I ../Library_UTF8-CPP/include \
           -I ../Library_JPEG/include \
           -I ../Library_MMDFiles/include \
           -I ../Library_GLFW/include \
           -I src/include \
           -I /usr/X11/include

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) cru $(TARGET) $(OBJECTS)
	cp src/include/*.h include

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.cpp=.o) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
