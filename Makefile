
MG_PRE_T = malaga

CWARNS=\
	-Wall \
	-Wextra \
	-Wshadow \
	-Wno-missing-braces

CWARNSNOTMAC=\
	-Wno-format-truncation

CFLAGS = $(CWARNS) -std=c99 -Iinclude/

ifeq ($(OS),Windows_NT)
	UNAME_S   = WINDOWS
	MYOS      = WINDOWS
	CC=tcc
	MYLDFLAGS = C:/raylib/raylib/src/raylib.rc.data -DPLATFORM_DESKTOP
	MYLIBS    = -lraylib -lmsvcrt -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32
	MG_T      = $(MG_PRE_T).exe
	RM        = rm -f
	CFLAGS += $(CWARNSNOTMAC)
else
	UNAME_S = $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CC=gcc
		MYLDFLAGS = -DPLATFORM_DESKTOP
		MYLIBS    = -lraylib -lm -lpthread -ldl
		MG_T      = $(MG_PRE_T)
		RM        = rm -f
		MYOS      = UNIX
		CFLAGS += $(CWARNSNOTMAC)
	endif
	ifeq ($(UNAME_S),Darwin)
		CC=clang
		MYLDFLAGS = -DPLATFORM_DESKTOP
		MYLIBS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lraylib
		MG_T      = $(MG_PRE_T)
		RM        = rm -f
		MYOS      = UNIX
	endif
endif

# find src/rlu | grep '\.c$' | sed 's/\.c/\.o \\/g'
RLU_O=\
src/rlu/anim.o \
src/rlu/anim_man.o \
src/rlu/atmosphere.o \
src/rlu/context.o \
src/rlu/input.o \
src/rlu/key_man.o \
src/rlu/key_object.o \
src/rlu/music_man.o \
src/rlu/parray.o \
src/rlu/scene.o \
src/rlu/scene_man.o \
src/rlu/scene_object.o \
src/rlu/texture_man.o \
src/rlu/ui.o \
src/rlu/util.o

# find src/modules | grep '\.c$' | sed 's/\.c/\.o \\/g'
CORE_O=\
src/modules/atmos.o \
src/modules/bullet.o \
src/modules/drop.o \
src/modules/game.o \
src/modules/levelselect.o \
src/modules/map.o \
src/modules/mg.o \
src/modules/pause.o \
src/modules/score.o

MG_O=src/main.o

ALL_O= $(MG_O) $(CORE_O) $(RLU_O)
ALL_T= $(MG_T)

.PHONY: clean cleansubs clobber

# TODO: Add 'subs' to the build dependency chain once it is needed
all: debug

release: CFLAGS += -O2
release: $(MG_T)

debug: CFLAGS += -ggdb -O0
debug: $(MG_T)

CORE_T: $(RLU_O) $(CORE_O)

$(MG_T): CORE_T $(MG_O)
	$(CC) -o $@ $(MYLDFLAGS) $(ALL_O) $(MYLIBS)

clean:
	$(RM) $(ALL_T) $(ALL_O)

subs: collections lua

cleansubs:
	make clean -C submodules/collections
	make clean -C submodules/lua

clobber: clean cleansubs

# find src | grep '\.c$' | xargs gcc -MM
main.o: src/main.c src/modules.h
atmos.o: src/modules/atmos.c src/modules/atmos.h src/modules/data.h
bullet.o: src/modules/bullet.c src/modules/bullet.h src/modules/score.h \
 src/modules/game.h
drop.o: src/modules/drop.c src/modules/drop.h src/modules/bullet.h
game.o: src/modules/game.c src/modules/../modules.h src/modules/data.h \
 src/modules/map.h src/modules/game.h src/modules/bullet.h \
 src/modules/score.h src/modules/atmos.h src/modules/drop.h
levelselect.o: src/modules/levelselect.c src/modules/../modules.h \
 src/modules/atmos.h src/modules/data.h
map.o: src/modules/map.c src/modules/map.h src/modules/game.h \
 src/modules/bullet.h src/modules/data.h
mg.o: src/modules/mg.c src/modules/atmos.h src/modules/../modules.h
pause.o: src/modules/pause.c src/modules/../modules.h
score.o: src/modules/score.c src/modules/score.h
anim.o: src/rlu/anim.c
anim_man.o: src/rlu/anim_man.c
atmosphere.o: src/rlu/atmosphere.c
context.o: src/rlu/context.c
input.o: src/rlu/input.c
key_man.o: src/rlu/key_man.c
key_object.o: src/rlu/key_object.c
music_man.o: src/rlu/music_man.c
parray.o: src/rlu/parray.c
scene.o: src/rlu/scene.c
scene_man.o: src/rlu/scene_man.c
scene_object.o: src/rlu/scene_object.c
texture_man.o: src/rlu/texture_man.c
ui.o: src/rlu/ui.c
util.o: src/rlu/util.c

# submodules

lua: submodules/lua/lua
submodules/lua/lua:
	submodules/build.sh -s $(MYOS) -m lua

collections: submodules/collections/build
submodules/collections/build:
	submodules/build.sh -s $(MYOS) -m collections
