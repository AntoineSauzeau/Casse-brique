#include "button_widget.h"

#include "colors.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

static Button** l_button = NULL;
static int n_button = 0;

/*
*	Prototypes.
*/


Button BTN_Create()
{
	Button button;
	memset(button.text, '\0', 50);
	button.font = NULL;
	button.padding = 15;
	button.border_thickness = 0;
	button.text_color = WHITE;
	button.pos.x = 100;
	button.pos.y = 100;

	return button;
}

void BTN_Show(Button* button)
{
	errno = 0;

	Button** ptr_tmp = (Button **) realloc(l_button, (n_button + 1) * sizeof(Button *));
	if (errno) {
		ShowError("Error realloc : %s \n", errno);

		free(l_button);
		ExitBreakout(EXIT_FAILURE);
	}

	l_button = ptr_tmp;

	n_button++;
	if (l_button != NULL) {
		l_button[n_button - 1] = button;
	}
}

void BTN_Hide(Button* button)
{
	errno = 0;

	bool shift = false;
	for (int i = 0; i < (n_button - 1); i++) {

		if (l_button[i] == button) {
			shift = true;
		}

		if (shift) {
			l_button[i] = l_button[i + 1];
		}
	}

	Button** ptr_tmp = realloc(l_button, (n_button - 1) * sizeof(Button*));
	if (errno) {
		ShowError("Error realloc : %s \n", errno);

		free(l_button);
		ExitBreakout(EXIT_FAILURE);
	}


	l_button = ptr_tmp;
	n_button--;
}

void BTN_HideAll()
{
	for (int i = 0; i < n_button; i++)
	{
		BTN_Hide(l_button[i]);
	}
}

void BTN_DrawAll(SDL_Renderer* renderer)
{
	for (int i = 0; i < n_button; i++) {
		BTN_Draw(l_button[i], renderer);
	}
}

void BTN_Draw(Button* button, SDL_Renderer* renderer)
{
	SDL_Surface* text_surface = TTF_RenderText_Blended(button->font, button->text, button->text_color);
	if (!text_surface) {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s \n", TTF_GetError());
		return;
	}

	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	SDL_Rect rect_text;
	rect_text.x = button->pos.x - text_surface->w / 2;
	rect_text.y = button->pos.y - text_surface->h / 2;
	rect_text.w = text_surface->w;
	rect_text.h = text_surface->h;

	SDL_RenderCopy(renderer, text_texture, NULL, &rect_text);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);


}
