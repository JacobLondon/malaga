
include ../include.mk

MG_T = $(PROJECT)$(MYEXT)

MG_O=main.o

# find . -type f | grep -v main | grep '\.c$' | sed 's/\.c/\.o \\/g'
CORE_O=\
atmos.o \
bullet.o \
drop_manager.o \
drop.o \
map.o \
mod_game.o \
mod_hiscore.o \
mod_levelselect.o \
mod_mg.o \
mod_pause.o \
mod_skins.o \
score.o \
scorefile.o

.PHONY: clean $(MG_T)

ALL_O = $(CORE_O) $(MG_O)

release: $(MG_T)
debug: $(MG_T)

# *MUST* be this order... at least for tcc
# all CC -o TARGET *.o *.data *.a -l* CFLAGS
$(MG_T): $(ALL_O)
	$(CC) -o $@ $(ALL_O) $(MYLIBS) $(MYLDFLAGS) $(CFLAGS)

clean:
	$(RM_F) *.o $(MG_T)

depend:
	find . | grep -E '\.c|\.h' | xargs $(CC) $(CFLAGS) -MM
