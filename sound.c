
#include "SDL/SDL_mixer.h"
#include <stdio.h>

static Mix_Music* music_background_1 = NULL;

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

	music_background_1 = Mix_LoadMUS("audio1.mp3");
	if (!music_background_1) {
		fprintf(stderr, "%s", Mix_GetError());
	}
}

void FreeMusics() {
	Mix_FreeMusic(music_background_1);
}

void Audio_PlayMusic(int music_index, int loop) {
	
	Mix_Music* music_to_play = NULL;
	if (music_index == 0) {
		music_to_play = music_background_1;
	}

	Mix_PlayMusic(music_to_play, loop);
}