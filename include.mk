
CWARNS=\
	-Wall \
	-Wextra \
	-Wshadow \
	-Wno-missing-braces

CWARNSNOTMAC=\
	-Wno-format-truncation

CFLAGS = $(CWARNS) -std=c99 -I..

AR = ar rc
RANLIB = ranlib
RM_F = rm -rf

ifeq ($(OS),Windows_NT)
	CFLAGS   += $(CWARNSNOTMAC)

	MYEXT     = .exe
	UNAME_S   = Windows
	CC        = tcc
	MYLDFLAGS = C:/raylib/raylib/src/raylib.rc.data -DPLATFORM_DESKTOP
	MYLIBS    = -lraylib -lmsvcrt -lopengl32 -lgdi32 -lwinmm -lkernel32 -lshell32 -luser32
else
	MYEXT =
	UNAME_S = $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CFLAGS   += $(CWARNSNOTMAC)

		CC        = gcc
		MYLDFLAGS = -DPLATFORM_DESKTOP
		MYLIBS    = -lraylib -lm -lpthread -ldl
	endif
	ifeq ($(UNAME_S),Darwin)
		CC        = clang
		MYLDFLAGS = -DPLATFORM_DESKTOP
		MYLIBS    = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lraylib
	endif
endif

RLU_A = ../rlu/librlu.a
MYLUA_A = ../lua/lua/liblua.a

MYLIBS += $(RLU_A) $(MYLUA_A)

all: debug
# required by all includers
release: CFLAGS += -O2
debug: CFLAGS += -ggdb -O0
