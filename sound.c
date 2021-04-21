
#include "SDL/SDL_mixer.h"
#include <stdio.h>

static Mix_Music* music_1 = NULL;
static Mix_Music* music_2 = NULL;

/*
	Prototypes.
*/
static void LoadMusics();
static void FreeMusics();

void Audio_InitModule() {

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
		fprintf(stderr, "%s", Mix_GetError());
	}

	LoadMusics();
}

void Audio_FreeModule() {
	FreeMusics();
	Mix_CloseAudio();
}

void LoadMusics() {

	music_1 = Mix_LoadMUS("music_1.ogg");
	if (!music_1) {
		fprintf(stderr, "Error Mix_LoadMUS : %s\n", Mix_GetError());
	}

	music_2 = Mix_LoadMUS("music_2.mp3");
	if (!music_2) {
		fprintf(stderr, "Error Mix_LoadMUS : % s\n", Mix_GetError());
	}
}

void FreeMusics() {
	Mix_FreeMusic(music_1);
}

void Audio_PlayMusic(int music_index, int loop) {
	
	Mix_Music* music_to_play = NULL;
	if (music_index == 0) {
		music_to_play = music_1;
	}
	else if (music_index == 1) {
		music_to_play = music_2;
	}

	Mix_PlayMusic(music_to_play, loop);
}