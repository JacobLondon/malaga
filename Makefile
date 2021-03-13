ifeq ($(OS),Windows_NT)
	CC=C:\raylib\tcc\tcc
	LDFLAGS = C:\raylib\raylib\src\raylib.rc.data -Os -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
	CFLAGS = -lmsvcrt -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32 -Wl -O2 -Iinclude/
	TARGET = test.exe
	REMOVE = del
else
	CC=tcc
	LDFLAGS = -O2 -std=c99 -Wall -Iexternal -DPLATFORM_DESKTOP
	CFLAGS = -lraylib -lm -O2 -Iinclude
	TARGET = test
	REMOVE = rm -f
endif

.PHONY: clean

SOURCES = $(wildcard src/rlu/*.c) $(wildcard src/*.c) $(wildcard src/modules/*.c)

$(TARGET): $(SOURCES)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(REMOVE) $(TARGET)

