#pragma once

#include "SDL.h"

void CreateMenuInterface(SDL_Renderer* renderer);
void DestroyMenuInterface();

void MenuInterfaceEvent(SDL_Event* event);
void UpdateMenuInterface();

void ChangeMenuPage(int page_index);