CC=gcc
TARGET=test
CFLAGS=\
	-Iinclude \
	-lraylib \
	-lm \
	-ggdb


.PHONY: clean

FILES=\
	src/main.o \
	src/rlu/anim.o \
	src/rlu/anim_man.o \
	src/rlu/key_object.o \
	src/rlu/scene.o \
	src/rlu/scene_man.o \
	src/rlu/scene_object.o \
	src/rlu/texture_man.o \
	src/rlu/ui.o \
	src/rlu/util.o

$(TARGET): $(FILES)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o src/*.o src/rlu/*.o

