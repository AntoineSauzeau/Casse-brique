#include "interface_menu.h"

#include "interface.h"
#include "colors.h"
#include "text_switch_widget.h"

static SDL_Renderer* renderer;

static TSW tsw_page;

/*
*	Prototypes. 
*/

static void DrawMenuInterface();
static void CreateTSWs();

void CreateMenuInterface(SDL_Renderer* _renderer)
{
	renderer = _renderer;

	CreateTSWs();

}

void DestroyMenuInterface()
{
	
}

void MenuInterfaceEvent(SDL_Event* event)
{

}

void UpdateMenuInterface()
{
	DrawMenuInterface();
}

void DrawMenuInterface() {
	SDL_SetRenderDrawColor(renderer, GREY.r, GREY.g, GREY.b, GREY.a);
	SDL_RenderClear(renderer);

	SDL_Rect background_2_rect = { 0, 0, INTERFACE_WIDTH, INTERFACE_HEIGHT};
	SDL_RenderFillRect(renderer, &background_2_rect);

	SDL_RenderPresent(renderer);
}

void CreateTSWs() {

	tsw_page = TSW_Create();
	TSW_AddValue(&tsw_page, "Touches");
	TSW_AddValue(&tsw_page, "Réglages");
	TSW_AddValue(&tsw_page, "Classement");
	TSW_AddValue(&tsw_page, "Sauvegardes");
	TSW_AddValue(&tsw_page, "Statistiques");
}