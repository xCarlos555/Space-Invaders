#ifndef SCORES_H
#define SCORES_H

#include "common.h"

void saveScore(const char *name, int score);

void showScores(SDL_Renderer *renderer, TTF_Font *font);

void clearScores();

void askPlayerName(SDL_Renderer *renderer, TTF_Font *font, char *nameOut);

#endif
