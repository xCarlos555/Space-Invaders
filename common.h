#ifndef COMMON_H
#define COMMON_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_BULLETS 10
#define ENEMY_ROWS 3
#define ENEMY_COLS 5
#define MAX_NAME 50
#define SCORES_FILE "scores.txt"

typedef struct {
    int x, y;
    bool active;
} Bullet;

typedef struct {
    int x, y;
    bool alive;
} Enemy;

#endif
