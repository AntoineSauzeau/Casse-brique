#include <SDL_ttf.h>

typedef struct {
	char** l_value;
	int n_value;
	int index;

	TTF_Font* font;
	SDL_Color color;
	int padding;
	double arrow_size_ratio;
	SDL_Point pos;


} TSW;

void TSW_InitModule(SDL_Renderer* renderer);
void TSW_FreeModule();

TSW TSW_Create();
void TSW_AddValue(TSW* tsw, char* value);
void TSW_RemoveValue(TSW* tsw, char* value);

void TSW_Next(TSW* tsw);
void TSW_Previous(TSW* tsw);

void TSW_Draw(TSW* tsw);
void TSW_Show(TSW* tsw);
void TSW_Hide(TSW* tsw);