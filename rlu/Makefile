
include ../include.mk

#CFLAGS += $(MYLDFLAGS)

# find . -type f | grep '\.c$' | sed 's/\.c/\.o \\/g'
RLU_O=\
anim.o \
anim_man.o \
atmosphere.o \
context.o \
input.o \
key_man.o \
key_object.o \
music_man.o \
parray.o \
scene.o \
scene_man.o \
scene_object.o \
texture_man.o \
ui.o \
util.o

.PHONY: clean $(RLU_A)

GEN_T=atmosphere.gen.h

release: $(RLU_A)
debug: $(RLU_A)

$(GEN_T):
	$(DEF) -f atmosphere.json -o $(GEN_T)

$(RLU_A): $(GEN_T) $(RLU_O)
	$(AR) $@ $(RLU_O)
	$(RANLIB) $@

clean:
	$(RM_F) $(RLU_A) $(RLU_O) $(GEN_T)

depend:
	find . | grep -E '\.c|\.h' | xargs $(CC) $(CFLAGS) -MM
