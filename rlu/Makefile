
include ../include.mk

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

parray.o: parray.c parray.h
scene_object.o: scene_object.h anim.h raylib.h
music_man.o: music_man.c rlu.h raylib.h atmosphere.h scene.h \
  scene_object.h anim.h scene_man.h anim_man.h texture_man.h util.h \
  context.h input.h key_object.h key_man.h music_man.h ui.h parray.h
input.o: input.h
scene_man.o: scene_man.c rlu.h raylib.h atmosphere.h scene.h \
  scene_object.h anim.h scene_man.h anim_man.h texture_man.h util.h \
  context.h input.h key_object.h key_man.h music_man.h ui.h parray.h
key_man.o: key_man.h
anim.o: anim.c rlu.h raylib.h atmosphere.h scene.h scene_object.h anim.h \
  scene_man.h anim_man.h texture_man.h util.h context.h input.h \
  key_object.h key_man.h music_man.h ui.h parray.h
util.o: util.c rlu.h raylib.h atmosphere.h scene.h scene_object.h anim.h \
  scene_man.h anim_man.h texture_man.h util.h context.h input.h \
  key_object.h key_man.h music_man.h ui.h parray.h
ui.o: ui.h raylib.h
atmosphere.o: atmosphere.h scene.h scene_object.h anim.h raylib.h \
  scene_man.h anim_man.h texture_man.h util.h
anim_man.o: anim_man.c rlu.h raylib.h atmosphere.h scene.h scene_object.h \
  anim.h scene_man.h anim_man.h texture_man.h util.h context.h input.h \
  key_object.h key_man.h music_man.h ui.h parray.h
key_object.o: key_object.h scene_object.h anim.h raylib.h
context.o: context.h
texture_man.o: texture_man.h util.h
scene.o: scene.c rlu.h raylib.h atmosphere.h scene.h scene_object.h \
  anim.h scene_man.h anim_man.h texture_man.h util.h context.h input.h \
  key_object.h key_man.h music_man.h ui.h parray.h
key_man.o: key_man.c rlu.h raylib.h atmosphere.h scene.h scene_object.h \
  anim.h scene_man.h anim_man.h texture_man.h util.h context.h input.h \
  key_object.h key_man.h music_man.h ui.h parray.h
scene_man.o: scene_man.h scene.h scene_object.h anim.h raylib.h \
  anim_man.h
input.o: input.c rlu.h raylib.h atmosphere.h scene.h scene_object.h \
  anim.h scene_man.h anim_man.h texture_man.h util.h context.h input.h \
  key_object.h key_man.h music_man.h ui.h parray.h
rlu.o: rlu.h raylib.h atmosphere.h scene.h scene_object.h anim.h \
  scene_man.h anim_man.h texture_man.h util.h context.h input.h \
  key_object.h key_man.h music_man.h ui.h parray.h
atmosphere.gen.o: atmosphere.gen.h
parray.o: parray.h
music_man.o: music_man.h
scene_object.o: scene_object.c rlu.h raylib.h atmosphere.h scene.h \
  scene_object.h anim.h scene_man.h anim_man.h texture_man.h util.h \
  context.h input.h key_object.h key_man.h music_man.h ui.h parray.h
util.o: util.h
ui.o: ui.c rlu.h raylib.h atmosphere.h scene.h scene_object.h anim.h \
  scene_man.h anim_man.h texture_man.h util.h context.h input.h \
  key_object.h key_man.h music_man.h ui.h parray.h
anim.o: anim.h raylib.h
key_object.o: key_object.c rlu.h raylib.h atmosphere.h scene.h \
  scene_object.h anim.h scene_man.h anim_man.h texture_man.h util.h \
  context.h input.h key_object.h key_man.h music_man.h ui.h parray.h
context.o: context.c rlu.h raylib.h atmosphere.h scene.h scene_object.h \
  anim.h scene_man.h anim_man.h texture_man.h util.h context.h input.h \
  key_object.h key_man.h music_man.h ui.h parray.h
atmosphere.o: atmosphere.c rlu.h raylib.h atmosphere.h scene.h \
  scene_object.h anim.h scene_man.h anim_man.h texture_man.h util.h \
  context.h input.h key_object.h key_man.h music_man.h ui.h parray.h \
  atmosphere.gen.h
anim_man.o: anim_man.h raylib.h
raylib.o: raylib.h
texture_man.o: texture_man.c rlu.h raylib.h atmosphere.h scene.h \
  scene_object.h anim.h scene_man.h anim_man.h texture_man.h util.h \
  context.h input.h key_object.h key_man.h music_man.h ui.h parray.h
scene.o: scene.h scene_object.h anim.h raylib.h