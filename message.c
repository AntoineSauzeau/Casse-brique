#include "message.h"

#include "SDL/SDL_image.h"

#include <string.h>
#include <stdio.h>

#include "colors.h"


Message** l_message_displayed = NULL;
SDL_mutex* l_message_mutex = NULL;
int n_message_displayed = 0;



/*
*	Prototypes.
*/

static int MessageWidth(SDL_Renderer* renderer, Message* message);
static int MessageHeight(SDL_Renderer* renderer, Message* message);

static void MSG_DrawImages(SDL_Renderer* renderer, Message* message);



void MSG_InitModule()
{
	l_message_mutex = SDL_CreateMutex();
	if (!l_message_mutex) {
		fprintf(stderr, "Couldn't create mutex\n");

		ExitBreakout(EXIT_FAILURE);
	}
}

void MSG_FreeModule()
{
	free(l_message_displayed);
	l_message_displayed = NULL;

	SDL_DestroyMutex(l_message_mutex);
}

Message MSG_Create()
{
	Message message;
	memset(message.title, '\0', 80);
	memset(message.subtitle, '\0', 80);
	message.title_color = WHITE;
	message.subtitle_color = WHITE;
	message.font_title = NULL;
	message.font_subtitle = NULL;
	message.background_color = TRANSPARENT;
	message.border_color = TRANSPARENT;
	message.border_thickness = 0;
	message.padding = 10;
	message.space_btw_titles = 15;
	message.position.x = 0;
	message.position.y = 0;

	message.l_message_image = NULL;
	message.n_image = 0;

	message.l_message_image_mutex = SDL_CreateMutex();
	if (!message.l_message_image_mutex) {
		fprintf(stderr, "Couldn't create mutex\n");
		ExitBreakout(EXIT_FAILURE);
	}

	return message;
}

void MSG_Show(Message* message) {

	errno = 0;

	if (SDL_LockMutex(l_message_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");
		ExitBreakout(EXIT_FAILURE);
	}

	if (!MSG_Displayed(message)) {

		n_message_displayed++;

		unsigned long long new_size = ((unsigned long long) n_message_displayed) * sizeof(Message*);

		Message* ptr_tmp = (Message*)realloc(l_message_displayed, new_size);
		if (!ptr_tmp) {

			ShowError("Erreur realloc : %s", errno);

			free(l_message_displayed);
			l_message_displayed = NULL;

			ExitBreakout(EXIT_FAILURE);
		}

		l_message_displayed = ptr_tmp;
		if (l_message_displayed != NULL) {
			*(l_message_displayed + n_message_displayed - 1) = message;
		}
	}

	SDL_UnlockMutex(l_message_mutex);
}

void MSG_Hide(Message* message) {

	errno = 0;

	if (SDL_LockMutex(l_message_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");
		ExitBreakout(EXIT_FAILURE);
	}

	if (MSG_Displayed(message)) {

		bool shift = false;
		for (int i = 0; i < n_message_displayed; i++) {

			if (MSG_Same(message, l_message_displayed[i])) {
				shift = true;
			}
			else if (shift) {
				*(l_message_displayed + i - 1) = *(l_message_displayed + i);
			}
		}

		n_message_displayed--;

		if (n_message_displayed != 0) {

			unsigned long long new_size = ((unsigned long long) n_message_displayed) * sizeof(Message*);

			Message* ptr_tmp = (Message*)realloc(l_message_displayed, new_size);
			if (ptr_tmp == NULL) {

				ShowError("Erreur realloc : %s", errno);

				free(l_message_displayed);
				l_message_displayed = NULL;

				ExitBreakout(EXIT_FAILURE);
			}

			l_message_displayed = ptr_tmp;
		}
	}

	SDL_UnlockMutex(l_message_mutex);
}

bool MSG_Displayed(Message* message)
{
	if (SDL_LockMutex(l_message_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");
		ExitBreakout(-1);
	}

	for (int i = 0; i < n_message_displayed; i++) {

		Message *message_a = l_message_displayed[i];

		if (MSG_Same(message, message_a)) {

			SDL_UnlockMutex(l_message_mutex);
			return true;
		}

	}

	SDL_UnlockMutex(l_message_mutex);
	return false;
}

bool MSG_Same(Message* m1, Message* m2) {

	if (SDL_LockMutex(l_message_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");
		ExitBreakout(EXIT_FAILURE);
	}

	bool r = strcmp(m1->title, m2->title) == 0
		&& strcmp(m1->subtitle, m2->subtitle) == 0
		&& SameColor(m1->title_color, m2->subtitle_color)
		&& SameColor(m1->subtitle_color, m2->subtitle_color)
		&& m1->font_title == m2->font_title
		&& m1->font_subtitle == m2->font_subtitle
		&& SameColor(m1->background_color, m2->background_color)
		&& SameColor(m1->border_color, m2->border_color)
		&& m1->border_thickness == m2->border_thickness
		&& m1->padding == m2->padding
		&& m1->space_btw_titles == m2->space_btw_titles
		&& m1->position.x == m2->position.x
		&& m1->position.y == m2->position.y;

	SDL_UnlockMutex(l_message_mutex);

	return r;
}

void MSG_AddImage(Message* message, MessageImage* m_image)
{
	errno = 0;

	if(SDL_LockMutex(message->l_message_image_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");
		ExitBreakout(EXIT_FAILURE);
	}

	unsigned long long new_size = message->n_image * sizeof(MessageImage*);

	MessageImage* ptr_tmp = (MessageImage*)realloc(message->l_message_image, new_size);
	if (!ptr_tmp) {

		ShowError("Erreur realloc : %s", errno);

		free(message->l_message_image);
		message->l_message_image = NULL;

		ExitBreakout(EXIT_FAILURE);
	}

	message->l_message_image = ptr_tmp;

	message->n_image++;
	if (message->l_message_image != NULL) {
		*(message->l_message_image + message->n_image - 1) = m_image;
	}

	SDL_UnlockMutex(message->l_message_image_mutex);
}

void MSG_HideAll()
{
	if (SDL_LockMutex(l_message_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");
		ExitBreakout(EXIT_FAILURE);
	}

	for (int i = 0; i < n_message_displayed; i++) {
		Message* message = l_message_displayed[i];
		MSG_Hide(message);
	}

	SDL_UnlockMutex(l_message_mutex);
}

void MSG_DrawImages(SDL_Renderer* renderer, Message* message)
{
	if (SDL_LockMutex(message->l_message_image_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");
		ExitBreakout(EXIT_FAILURE);
	}

	for (int i = 0; i < message->n_image; i++) {

		SDL_Texture* texture_image = message->l_message_image[i]->texture;
		SDL_Rect image_position = message->l_message_image[i]->position;

		SDL_RenderCopy(renderer, texture_image, NULL, &image_position);
	}

	SDL_UnlockMutex(message->l_message_image_mutex);
}

void MSG_RemoveImage(Message* message, MessageImage* m_image)
{
	errno = 0;

	if (SDL_LockMutex(message->l_message_image_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");
		ExitBreakout(EXIT_FAILURE);
	}

	message->n_image--;
	if (message->n_image == 0) {
		free(message->l_message_image);
		message->l_message_image = NULL;
	}

	bool shift = false;
	for (int i = 0; i < message->n_image; i++) {

		if (message->l_message_image[i] == m_image) {
			shift = true;
		}
		else if (shift) {
			*(message->l_message_image + i - 1) = *(message->l_message_image + i);
		}
	}
	
	if (message->n_image != 0) {
		unsigned long long new_size = message->n_image * sizeof(MessageImage*);

		MessageImage* ptr_tmp = realloc(message->l_message_image, new_size);
		if (!ptr_tmp) {

			ShowError("Erreur realloc : %s", errno);

			free(message->l_message_image);
			message->l_message_image = NULL;

			ExitBreakout(EXIT_FAILURE);
		}
	}

	SDL_UnlockMutex(message->l_message_image_mutex);
}

void MSG_Draw(SDL_Renderer* renderer, Message* message)
{
	if (SDL_LockMutex(l_message_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");

		ExitBreakout(EXIT_FAILURE);
	}

	int message_width = MessageWidth(renderer, message);
	int message_height = MessageHeight(renderer, message);

	if (message_width == 0 || message_height == 0) {
		return;
	}

	if (message->font_title != NULL) {

		SDL_Surface* txt_title_surface = TTF_RenderText_Blended(message->font_title, message->title, message->title_color);
		if (txt_title_surface != NULL) {

			SDL_Texture* txt_title_texture = SDL_CreateTextureFromSurface(renderer, txt_title_surface);

			SDL_Rect txt_title_pos;
			txt_title_pos.x = message->position.x - txt_title_surface->w / 2 + message->padding;
			txt_title_pos.y = message->position.y - message_height / 2 + message->padding;
			txt_title_pos.w = txt_title_surface->w;
			txt_title_pos.h = txt_title_surface->h;

			SDL_RenderCopy(renderer, txt_title_texture, NULL, &txt_title_pos);

			SDL_FreeSurface(txt_title_surface);
			SDL_DestroyTexture(txt_title_texture);

		}
		else {
			SDL_Log("Error TTF_RenderText_Blended : %s", TTF_GetError());
		}
	}

	if (message->font_subtitle != NULL) {

		SDL_Surface* txt_subtitle_surface = TTF_RenderText_Blended(message->font_subtitle, message->subtitle, message->subtitle_color);
		if (txt_subtitle_surface != NULL) {

			SDL_Texture* txt_subtitle_texture = SDL_CreateTextureFromSurface(renderer, txt_subtitle_surface);

			SDL_Rect txt_subtitle_pos;
			txt_subtitle_pos.x = message->position.x - txt_subtitle_surface->w / 2 + message->padding;
			txt_subtitle_pos.y = message->position.y - message_height / 2 + message->padding + message->space_btw_titles;
			txt_subtitle_pos.w = txt_subtitle_surface->w;
			txt_subtitle_pos.h = txt_subtitle_surface->h;

			SDL_RenderCopy(renderer, txt_subtitle_texture, NULL, &txt_subtitle_pos);

			SDL_FreeSurface(txt_subtitle_surface);
			SDL_DestroyTexture(txt_subtitle_texture);

		}
		else {
			SDL_Log("Error TTF_RenderText_Blended : %s", TTF_GetError());
		}
	}

	MSG_DrawImages(renderer, message);

	SDL_UnlockMutex(l_message_mutex);
}

void MSG_DrawAll(SDL_Renderer* renderer)
{
	if (SDL_LockMutex(l_message_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");

		ExitBreakout(EXIT_FAILURE);
	}

	for (int i = 0; i < n_message_displayed; i++) {
		MSG_Draw(renderer, l_message_displayed[i]);
	}

	SDL_UnlockMutex(l_message_mutex);
}

int MessageWidth(SDL_Renderer* renderer, Message* message) {

	if (SDL_LockMutex(l_message_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");

		ExitBreakout(EXIT_FAILURE);
	}

	int title_width = 0;
	int subtitle_width = 0;

	if (message->font_title != NULL && strcmp(message->title, "") != 0) {

		SDL_Surface* txt_title_surface = TTF_RenderText_Blended(message->font_title, message->title, message->title_color);
		if (txt_title_surface != NULL) {
			title_width = txt_title_surface->w;

			SDL_FreeSurface(txt_title_surface);
		}
		else {
			SDL_Log("Error TTF_RenderText_Blended (MessageWidth) : %s", TTF_GetError());
		}
	}

	if (message->font_subtitle != NULL && strcmp(message->subtitle, "") != 0) {

		SDL_Surface* txt_subtitle_surface = TTF_RenderText_Blended(message->font_subtitle, message->subtitle, message->subtitle_color);
		if (txt_subtitle_surface != NULL) {
			subtitle_width = txt_subtitle_surface->w;

			SDL_FreeSurface(txt_subtitle_surface);
		}
		else {
			SDL_Log("Error TTF_RenderText_Blended (MessageWidth) : %s", TTF_GetError());
		}
	}

	int message_width;
	if (title_width > subtitle_width) {
		message_width= message->padding * 2 + title_width;
	}
	else {
		message_width =  message->padding * 2 + subtitle_width;
	}

	SDL_UnlockMutex(l_message_mutex);

	return message_width;
}

int MessageHeight(SDL_Renderer* renderer, Message* message) {

	if (SDL_LockMutex(l_message_mutex) != 0) {
		fprintf(stderr, "Couldn't lock mutex\n");

		ExitBreakout(EXIT_FAILURE);
	}

	int title_height = 0;
	int subtitle_height = 0;

	if (message->font_title != NULL && strcmp(message->title, "") != 0) {

		SDL_Surface* txt_title_surface = TTF_RenderText_Blended(message->font_title, message->title, message->title_color);
		if (txt_title_surface != NULL) {
			title_height = txt_title_surface->h;
		}
		else {
			SDL_Log("Error TTF_RenderText_Blended : %s", TTF_GetError());
		}

		SDL_FreeSurface(txt_title_surface);
	}

	if (message->font_subtitle != NULL && strcmp(message->subtitle, "") != 0) {

		SDL_Surface* txt_subtitle_surface = TTF_RenderText_Blended(message->font_subtitle, message->subtitle, message->subtitle_color);
		if (txt_subtitle_surface != NULL) {
			subtitle_height = txt_subtitle_surface->h;
		}
		else {
			SDL_Log("Error TTF_RenderText_Blended : %s", TTF_GetError());
		}

		SDL_FreeSurface(txt_subtitle_surface);
	}

	int message_height = message->padding * 2 + subtitle_height + subtitle_height + message->space_btw_titles;

	SDL_UnlockMutex(l_message_mutex);

	return message_height;
}