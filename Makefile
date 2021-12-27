UNAME := $(shell uname -o)

TARGET_EXEC ?= toost

BUILD_DIR ?= ./bin
SRC_DIRS ?= ./src

ifeq ($(PLATFORM),web)
	CC := emcc
	CXX := em++
else
	CC := gcc
	CXX := c++
endif

CFLAGS := -std=gnu11
CXXFLAGS := -std=gnu++20 -I./src
CPPFLAGS := -Wall -Wextra -Wno-missing-field-initializers -Wno-deprecated-enum-enum-conversion

ifeq ($(BUILD),release)
	# "Release" build - optimization, and no debug symbols
	CPPFLAGS += -O3 -s
else
	# "Debug" build - no optimization, and debugging symbols
	ifeq ($(PLATFORM),web)
	CPPFLAGS += -O0 -g
	else
	CPPFLAGS += -Og -g -ggdb
	endif
endif

ifeq ($(PLATFORM),web)
	PRELOADED_FILES = --preload-file img@/img --preload-file fonts@/fonts

	EMS += -s USE_SDL=2 -s USE_ZLIB=1 -s USE_FREETYPE=1 -s USE_LIBPNG=1 -s DISABLE_EXCEPTION_CATCHING=1
	CPPFLAGS += -I./third_party/cairo-1.16.0/src -O2 $(EMS)
	LDFLAGS += -s CASE_INSENSITIVE_FS=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1 -s TOTAL_STACK=10000000 -s ASYNCIFY -s FETCH=1 -s FORCE_FILESYSTEM=1 -o bin/index.html --shell-file shell_minimal.html $(PRELOADED_FILES) $(EMS) -L./third_party/cairo-1.16.0/src/.libs -lcairo -L./third_party/pixman-0.40.0/pixman/.libs -lpixman-1
else
	CPPFLAGS += $(shell pkg-config --cflags sdl2 glew glfw3 zlib cairo freetype2 libcurl) -Wno-cast-function-type
	LDFLAGS := -lpthread -lopengl32 $(shell pkg-config --libs --static sdl2 glew glfw3 zlib cairo freetype2 libcurl) -lunistring -liconv -lbrotlicommon -mconsole -fPIC -static -static-libgcc -static-libstdc++

	ifeq ($(UNAME),Msys)
		LDFLAGS += -lmingw32 -lws2_32 -lwsock32
	else
		LDFLAGS += -ldl
	endif
endif


SRCS := ./src/main.cpp ./src/LevelParser.cpp ./src/Drawers.cpp ./src/Helpers.cpp ./src/imgui/imgui.cpp ./src/imgui/imgui_widgets.cpp ./src/imgui/imgui_tables.cpp ./src/imgui/imgui_impl_sdl.cpp ./src/imgui/imgui_impl_opengl3.cpp ./src/imgui/imgui_draw.cpp ./src/SMM2CourseDecryptor/aes.cpp ./src/SMM2CourseDecryptor/decrypt.cpp ./src/fmt/format.cpp ./src/fmt/os.cpp

ifeq ($(UNAME)$(PLATFORM),Msys)
SRCS += src/info.rc src/icon.rc
endif

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
ifeq ($(PLATFORM),web)
	cp js/FileSaver.min.js bin
endif

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

ifeq ($(UNAME),Msys)
# Windows RES file
$(BUILD_DIR)/%.rc.o: %.rc
	$(MKDIR_P) $(dir $@)
	windres $< $@
endif

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
