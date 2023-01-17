APP=cannon

# Specify compilation flags
CXXFLAGS=-O0 -g
CPPFLAGS=-Iinclude -Iexternals/include -MMD
EXTRA_FLAGS=-Wall -Wextra -Wno-unused-function -Wno-unused-parameter

# Specify your compilation objects
OBJS=src/main.o src/app.o src/cannon.o src/imgui_utils.o src/Projectile.o

# No changes under this line

## Platform specific configuration
TARGET?=$(shell $(CXX) -dumpmachine)
LDFLAGS=-Lexternals/libs/$(TARGET)
ifeq ($(TARGET),x86_64-pc-cygwin)
externals/src/imgui_impl_opengl3.o: CXXFLAGS+=-D_WIN32
$(APP): LDLIBS+=-lglfw3 -lgdi32 -lstdc++ -lopengl32
else ifeq ($(TARGET),x86_64-w64-mingw32)
$(APP): LDLIBS+=-lglfw3 -lgdi32 -lstdc++ -lopengl32
else
$(APP): LDLIBS+=-lglfw3 -lpthread -ldl -lm -lstdc++ -lGL
endif

# External objects
EXT_OBJS=imgui.o imgui_demo.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui_impl_glfw.o imgui_impl_opengl3.o
EXT_OBJS+=stb_image.o
EXT_OBJS:=$(addprefix externals/src/,$(EXT_OBJS))

DEPS=$(OBJS:.o=.d) $(EXT_OBJS:.o=.d)

.PHONY: clean all

all: $(APP)

$(OBJS): CXXFLAGS+=$(EXTRA_FLAGS)

-include $(DEPS)

$(APP): $(OBJS) $(EXT_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(APP) $(OBJS) $(DEPS) $(EXT_OBJS) imgui.ini

