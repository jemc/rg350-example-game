BIN := example-game

CONFIG := host
include config/$(CONFIG).mk

SYSROOT     := /
SDL2_CFLAGS := $(shell $(SYSROOT)/usr/bin/sdl2-config --cflags) -DSDL_2
SDL2_LIBS   := $(shell $(SYSROOT)/usr/bin/sdl2-config --libs)

OBJS    := $(shell find src lib -name *.c | sed "s:c$$:o:")
INCLUDE := -I. -I./lib
DEFS    +=
LDFLAGS := -lm
CFLAGS   = -Wall -Wno-unused-variable \
					 -g -O0 -fomit-frame-pointer $(DEFS) $(INCLUDE)

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	rm -f $(OBJS) $(BIN) $(BIN).opk

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SDL2_CFLAGS) $(SDL2_LIBS) -o $@ $^

$(BIN).opk: $(BIN)
	rm -rf .opk_data
	cp -r data .opk_data
	cp $^ .opk_data
	mksquashfs .opk_data $@ -all-root -noappend -no-exports -no-xattrs -no-progress >/dev/null
