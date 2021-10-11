
PROJECT = malaga

CWARNS=\
	-Wall \
	-Wextra \
	-Wshadow \
	-Wno-missing-braces

CWARNSNOTMAC=\
	-Wno-format-truncation

CFLAGS = $(CWARNS) -std=c99 -I.. -I../lua

AR = ar rc
RANLIB = ranlib
RM_F = rm -rf

default_target = debug
found = No

RLU_A = ../rlu/librlu.a
MYLUA_A = ../lua/lua/liblua.a
LUA = ../lua/lua/lua$(MYEXT)
DEF = ../collections/def$(MYEXT)

MYLIBS = $(RLU_A) $(MYLUA_A)

ifeq ($(OS),Windows_NT)
	found    = Yes
	CFLAGS   += $(CWARNSNOTMAC)
	AR       = tcc -ar rc

	MYEXT     = .exe
	UNAME_S   = Windows
	CC        = tcc
	MYLDFLAGS = -DPLATFORM_DESKTOP
	MYLIBS   += C:/raylib/raylib/src/raylib.rc.data -lmsvcrt -lraylib -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32
else
	MYEXT =
	UNAME_S = $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		found    = Yes
		CFLAGS   += $(CWARNSNOTMAC)

		CC        = gcc
		MYLDFLAGS = -DPLATFORM_DESKTOP
		MYLIBS   += -lraylib -lm -lpthread -ldl
	endif
	ifeq ($(UNAME_S),Darwin)
		found     = Yes
		CC        = clang
		MYLDFLAGS = -DPLATFORM_DESKTOP
		MYLIBS   += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lraylib
	endif
endif

ifeq ($(found),No)
	default_target = abort
endif

all: $(default_target)
# required by all includers
release: CFLAGS += -O2
debug: CFLAGS += -ggdb -O0

abort:
	@echo 'ERROR: Your OS isn't supported
	@false
