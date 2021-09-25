
include ../include.mk

MG_T = $(PROJECT)$(MYEXT)

MG_O=main.o

# find . -type f | grep -v main | grep '\.c$' | sed 's/\.c/\.o \\/g'
CORE_O=\
game.o \
bullet.o \
levelselect.o \
drop.o \
mg.o \
score.o \
map.o \
pause.o \
atmos.o

ALL_O = $(CORE_O) $(MG_O)

release: $(MG_T)
debug: $(MG_T)

$(MG_T): $(CORE_O) $(MG_O)
	$(CC) -o $@ $(MYLDFLAGS) $(ALL_O) $(MYLIBS) $(CFLAGS)

clean:
	$(RM_F) $(ALL_O) $(MG_T)

depend:
	find . | grep -E '\.c|\.h' | xargs $(CC) $(CFLAGS) -MM
