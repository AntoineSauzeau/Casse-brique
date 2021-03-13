#pragma once

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game.h"

#define INTERFACE_WIDTH 900
#define INTERFACE_HEIGHT 575

#define FPS 120

const static SDL_Color RED = { 235, 77, 75,1.0 };
const static SDL_Color GREY = { 46, 40, 42 };
const static SDL_Color GREEN_1 = { 46, 204, 113 }; //EMERALD
const static SDL_Color BLACK =  { 0, 0, 0 };
const static SDL_Color BLUE_1 = { 52, 152, 219 };
const static SDL_Color WHITE = { 255, 255, 255 };
const static SDL_Color VIOLET = { 44, 62, 80, 1.0 };
const static SDL_Color ORANGE = { 243, 156, 18,1.0 };

void CreateInterface();
void DestroyInterface();

void LoadFonts();
void FreeFonts();

void LoadImages();
void FreeImages();

void StartEventLoop();
void StartGameLoop();

void GameLoop();

void Draw();
int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);
