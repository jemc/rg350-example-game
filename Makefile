BIN := example-game

SYSROOT := /

CONFIG := host
include config/$(CONFIG).mk

SDL2_CFLAGS := $(shell $(SYSROOT)/usr/bin/sdl2-config --cflags) -DSDL_2
SDL2_LIBS   := $(shell $(SYSROOT)/usr/bin/sdl2-config --libs) -lSDL2_image

SRC_OBJS := $(shell find src -name *.c | sed "s:c$$:o:")
LIB_OBJS := $(shell find lib -name *.c | sed "s:c$$:o:")
OBJS     := $(SRC_OBJS) $(LIB_OBJS)
SRC_HEADERS := $(shell find src -name *.h)
LIB_HEADERS := $(shell find lib -name *.h)
HEADERS     := $(SRC_HEADERS) $(LIB_HEADERS)

RAW_ROOM := $(shell find src/world/room -name *-raw)
GEN_ROOM_H := $(shell find src/world/room -name *-raw | sed "s:-raw$$:.h:")
GEN_ROOM_XPM := $(shell find src/world/room -name *-raw | sed "s:-raw$$:.xpm:")
GEN_ROOM_H_SCRIPT := util/room_raw_to_h.jq

INCLUDE  := -I. -I./lib
DEFS     +=
LDFLAGS  := -lm
CFLAGS    = -Wall -Wno-unused-variable \
            -g -O0 -fomit-frame-pointer $(DEFS) $(INCLUDE)

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	rm -f $(OBJS) $(BIN) $(BIN).opk

# In addition to the standard Makefile rule of object files being built from
# their corresponding C files, we make it here so that they also have a
# freshness dependency of any header files they might possibly use.
# We don't know here which headers are specifically used by which C files,
# so we use a general rule that src objects depend on src and lib headers,
# whereas lib objects only depend on lib headers and never on src headers.
$(SRC_OBJS): $(LIB_HEADERS) $(SRC_HEADERS)
$(LIB_OBJS): $(LIB_HEADERS)

# The source objects also depend on room data, which is generated from raw
# JSON/PNG exports from Tilesetter, a program used for easy tiling and mapping.
# The raw exports are not checked into this git repo, and we are instead
# checking in just the generated headers and XPMs to compile as source code.
# However, if new raw exports are done, then this Makefile should convert them
# into headers and XPMs to replace the outdated ones previously generated,
# so this Makefile looks for such raw exports being available and uses them.
$(SRC_OBJS): $(GEN_ROOM_H) $(GEN_ROOM_XPM)

# The room headers are generated from the JSON part of the raw exports.
# We use a tailor-made `jq` script to turn them into C-syntax header files.
$(GEN_ROOM_H): $(RAW_ROOM)
	cat $(shell echo $@ | sed "s:.h$$:-raw:")/*.txt \
	| env ROOM_ID=$(shell echo $@ | sed "s:.h$$::" | xargs basename) \
		jq -r -f $(GEN_ROOM_H_SCRIPT) \
	> $@

# The room tileset XPM files are generated from the PNG part of the raw exports.
# We use the `convert` CLI tool from the ImageMagick utils package to do this.
$(GEN_ROOM_XPM): $(RAW_ROOM)
	$(eval name := $(shell echo $@ | sed "s:.xpm$$::"))
	$(eval dirname := $(shell dirname $(name)))
	$(eval basename := $(shell basename $(name)))
	convert $(name)-raw/*.png $(dirname)/room_$(basename)_tileset_data.xpm
	mv $(dirname)/room_$(basename)_tileset_data.xpm $@

# The program binary is produced by linking the compiled object files.
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SDL2_CFLAGS) $(SDL2_LIBS) -o $@ $^

# The OPK package (for RG350) is produced using squashfs.
$(BIN).opk: $(BIN)
	rm -rf .opk_data
	cp -r data .opk_data
	cp $^ .opk_data
	mksquashfs .opk_data $@ -all-root -noappend -no-exports -no-xattrs -no-progress >/dev/null
