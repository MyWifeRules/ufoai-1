TARGET             := ufo2map

# if the linking should be static
$(TARGET)_STATIC   ?= $(STATIC)
ifeq ($($(TARGET)_STATIC),1)
$(TARGET)_LDFLAGS  += -static
endif

$(TARGET)_LINKER   := $(CXX)
$(TARGET)_FILE     := $(TARGET)$(EXE_EXT)
$(TARGET)_CFLAGS   += -DCOMPILE_MAP -ffloat-store $(SDL_CFLAGS)
$(TARGET)_LDFLAGS  += -lm -lpng -ljpeg -lz $(SDL_LIBS)

ifeq ($(SSE),1)
   $(TARGET)_CFLAGS := $(filter-out -ffloat-store,$($(TARGET)_CFLAGS))
endif

$(TARGET)_SRCS      = \
	tools/ufo2map/ufo2map.cpp \
	tools/ufo2map/lighting.cpp \
	tools/ufo2map/bsp.cpp \
	tools/ufo2map/bspbrush.cpp \
	tools/ufo2map/csg.cpp \
	tools/ufo2map/faces.cpp \
	tools/ufo2map/levels.cpp \
	tools/ufo2map/lightmap.cpp \
	tools/ufo2map/map.cpp \
	tools/ufo2map/patches.cpp \
	tools/ufo2map/portals.cpp \
	tools/ufo2map/routing.cpp \
	tools/ufo2map/textures.cpp \
	tools/ufo2map/tree.cpp \
	tools/ufo2map/threads.cpp \
	tools/ufo2map/writebsp.cpp \
	tools/ufo2map/check/checkentities.cpp \
	tools/ufo2map/check/checklib.cpp \
	tools/ufo2map/check/check.cpp \
	tools/ufo2map/common/aselib.cpp \
	tools/ufo2map/common/bspfile.cpp \
	tools/ufo2map/common/polylib.cpp \
	tools/ufo2map/common/scriplib.cpp \
	tools/ufo2map/common/trace.cpp \
	\
	shared/mathlib.cpp \
	shared/mutex.cpp \
	shared/byte.cpp \
	shared/images.cpp \
	shared/parse.cpp \
	shared/shared.cpp \
	shared/entitiesdef.cpp \
	shared/utf8.cpp \
	\
	common/files.cpp \
	common/list.cpp \
	common/mem.cpp \
	common/unzip.cpp \
	common/tracing.cpp \
	common/routing.cpp \
	common/ioapi.cpp

ifeq ($(TARGET_OS),mingw32)
	$(TARGET)_SRCS+=\
		ports/windows/win_shared.cpp
else
	$(TARGET)_SRCS+= \
		ports/unix/unix_files.cpp \
		ports/unix/unix_shared.cpp \
		ports/unix/unix_main.cpp
endif

$(TARGET)_OBJS     := $(call ASSEMBLE_OBJECTS,$(TARGET))
$(TARGET)_CXXFLAGS := $($(TARGET)_CFLAGS)
$(TARGET)_CCFLAGS  := $($(TARGET)_CFLAGS)
