#include "message.h"

struct Message CreateMessage()
{
	struct Message message;
	message.title = "Title";
	message.subtitle = "Subtitle";
	message.title_color = BLACK;
	message.subtitle_color = BLACK;
	message.title_size = 22;
	message.subtitle_size = 18;
	message.font = NULL;
	message.background_color = TRANSPARENT;
	message.border_color = TRANSPARENT;
	message.border_thickness = 0;
	message.padding = 10;
	message.space_btw_titles = 15;
	message.position.x = 0;
	message.position.y = 0;

	return message;
}

void DrawMessage(SDL_Renderer* renderer, struct Message* message)
{
	if (!message->font) {
		return;
	}

	int message_width = MessageWidth(message);
	int message_height = MessageHeight(message);

	SDL_Surface* txt_title_surface = TTF_RenderText_Blended(renderer, message->title, message->title_color);
	if (txt_title_surface != NULL) {

		SDL_Texture* txt_title_texture = SDL_CreateTextureFromSurface(renderer, txt_title_surface);



		SDL_Rect txt_title_pos;
		txt_title_pos.x = message->position.x - message_width / 2 + message->padding;
		txt_title_pos.y = message->position.y - message_height / 2 + message->padding;
		txt_title_pos.w = txt_title_surface->w;
		txt_title_pos.h = txt_title_surface->h;

		SDL_RenderCopy(renderer, txt_title_texture, &txt_title_pos, NULL);

		SDL_FreeSurface(txt_title_surface);
		SDL_DestroyTexture(txt_title_texture);

	}
	else {
		SDL_Log("Error TTF_RenderText_Blended : %s", SDL_GetError());
	}

	SDL_Surface* txt_subtitle_surface = TTF_RenderText_Blended(renderer, message->subtitle, message->subtitle_color);
	if (txt_subtitle_surface != NULL) {

		SDL_Texture* txt_subtitle_texture = SDL_CreateTextureFromSurface(renderer, txt_subtitle_surface);



		SDL_Rect txt_subtitle_pos;
		txt_subtitle_pos.x = message->position.x - message_width / 2 + message->padding;
		txt_subtitle_pos.y = message->position.y - message_height / 2 + message->padding;
		txt_subtitle_pos.w = txt_subtitle_surface->w;
		txt_subtitle_pos.h = txt_subtitle_surface->h;

		SDL_RenderCopy(renderer, txt_subtitle_texture, &txt_subtitle_pos, NULL);

		SDL_FreeSurface(txt_subtitle_surface);
		SDL_DestroyTexture(txt_subtitle_texture);

	}
	else {
		SDL_Log("Error TTF_RenderText_Blended : %s", SDL_GetError());
	}

}

int MessageWidth(struct Message* message) {

}

int MessageHeight(struct Message* message) {

}