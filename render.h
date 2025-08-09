#ifndef RENDER_H
#define RENDER_H

#include "common.h"

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y);

void renderTextCentered(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect rect);

#endif
