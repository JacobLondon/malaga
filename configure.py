#!/usr/bin/env python3

import platform

try:
    fp = open("Makefile", "w")
except:
    exit(1)

OS = platform.system()
if OS in ("Linux", "Darwin"):
    print("""\
CC=gcc
LDFLAGS = -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
CFLAGS = -lraylib -lm -lpthread -ldl -Iinclude -Wall -ggdb -Wno-format-truncation -Wno-missing-braces
TARGET = test
REMOVE = rm -f

.PHONY: clean

SOURCES = $(wildcard src/rlu/*.c) $(wildcard src/*.c) $(wildcard src/modules/*.c)
OBJECTS = ${SOURCES:c=o}

$(TARGET): $(OBJECTS)
\t$(CC) -o $@ $^ $(CFLAGS)

clean:
\t$(REMOVE) $(TARGET) $(OBJECTS)
""", file=fp)

elif OS == "Windows":
    # just use git bash or something...
    print("""\
CC=C:\\raylib\\tcc\\tcc
LDFLAGS = C:\\raylib\\raylib\src\\raylib.rc.data -Os -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
CFLAGS = -lmsvcrt -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32 -Wl -O2 -Iinclude/ -ggdb
TARGET = test.exe
REMOVE = rm

.PHONY: clean

SOURCES = $(wildcard src/rlu/*.c) $(wildcard src/*.c) $(wildcard src/modules/*.c)

$(TARGET): $(SOURCES)
\t$(CC) -o $@ $^ $(CFLAGS)

clean:
\t$(REMOVE) $(TARGET)
""", file=fp)

fp.close()
