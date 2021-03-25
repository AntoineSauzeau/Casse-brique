#pragma once

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "colors.h"
#include "message.h"

#define INTERFACE_WIDTH 900
#define INTERFACE_HEIGHT 575

#define FPS 120

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
bool SameColor(SDL_Color c1, SDL_Color c2);

void CreateMessages();

void ShowStartMessage();
void HideStartMessage();
void ShowSpeed3Message();
void HideSpeed3Message();
void ShowGameoverMessage();
void HideGameoverMessage();
void ShowNextlevelMessage(int level);
void HideNextlevelMessage();
