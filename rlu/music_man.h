#ifndef RLU_MUSIC_MAN_H
#define RLU_MUSIC_MAN_H

#include "raylib.h"

typedef struct sound_def_tag {
	const char *name;
	const char *filename;
	Sound sound;
	bool loaded;
} sound_def;

#define DEFINE_SOUND(NAME, PATH) \
{ \
	.name=NAME, \
	.filename=PATH, \
}

typedef struct music_def_tag {
	const char *name;
	const char *filename;
	Music music;
	bool loaded;
} music_def;

#define DEFINE_MUSIC(NAME, PATH) \
{ \
	.name=NAME, \
	.filename=PATH, \
}

/**
 * initialize the music module
 */
void music_man_init(sound_def *sounds, music_def *songs);

/**
 * clean up the music module
 */
void music_man_cleanup(void);

/**
 * Stop all music and sounds and unload them
 */
void music_man_unload(void);

void music_man_update(void);


void music_man_set_volume(float volume);

float music_man_get_volume(void);

/**
 * Load a sound, it cannot be played until this happens.
 * Return the UID of the sound to be passed into play_sound
 */
int music_man_load_sound(const char *name);

/**
 * Play a sound once, the sound must have been loaded
 * on init
 */
void music_man_play_sound(int uid);

/**
 * Play music from a file on loop until cleanup
 */
void music_man_load_music(const char *name);

#endif // RLU_MUSIC_MAN_H
