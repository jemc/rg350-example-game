BIN := example-game

CONFIG := host
include config/$(CONFIG).mk

SYSROOT     := /
SDL2_CFLAGS := $(shell $(SYSROOT)/usr/bin/sdl2-config --cflags) -DSDL_2
SDL2_LIBS   := $(shell $(SYSROOT)/usr/bin/sdl2-config --libs) -lSDL2_image

SRC_OBJS := $(shell find src -name *.c | sed "s:c$$:o:")
LIB_OBJS := $(shell find lib -name *.c | sed "s:c$$:o:")
OBJS     := $(SRC_OBJS) $(LIB_OBJS)
SRC_HEADERS := $(shell find src -name *.h)
LIB_HEADERS := $(shell find lib -name *.h)
HEADERS     := $(SRC_HEADERS) $(LIB_HEADERS)

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

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SDL2_CFLAGS) $(SDL2_LIBS) -o $@ $^

$(BIN).opk: $(BIN)
	rm -rf .opk_data
	cp -r data .opk_data
	cp $^ .opk_data
	mksquashfs .opk_data $@ -all-root -noappend -no-exports -no-xattrs -no-progress >/dev/null
