#include "game.h"
#include "render.h"
#include <SDL_ttf.h>

int playGame(const char *name, SDL_Renderer *renderer) {
    int playerX = SCREEN_WIDTH / 2 - 25;
    int playerY = SCREEN_HEIGHT - 50;
    int playerSpeed = 5;

    Bullet bullets[MAX_BULLETS] = {0};
    int bulletSpeed = 8;

    Enemy enemies[ENEMY_ROWS][ENEMY_COLS];
    int enemyWidth = 50;
    int enemyHeight = 20;
    int enemySpacing = 20;
    int enemyStartX = 100;
    int enemyStartY = 50;
    int enemySpeedX = 2;
    int enemyDirection = 1;

    SDL_Rect btnBack = { SCREEN_WIDTH - 90, 20, 60, 25 };

    // inicializar inimigos
    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            enemies[row][col].x = enemyStartX + col * (enemyWidth + enemySpacing);
            enemies[row][col].y = enemyStartY + row * (enemyHeight + enemySpacing);
            enemies[row][col].alive = true;
        }
    }

    int score = 0;
    bool running = true;
    bool gameOver = false;
    SDL_Event event;

    TTF_Font *font = TTF_OpenFont("cour.ttf", 12);
    if (!font) {
        SDL_Log("Erro ao carregar fonte no playGame: %s", TTF_GetError());
        return score;
    }

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (!gameOver && event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bullets[i].active) {
                        bullets[i].x = playerX + 25 - 2;
                        bullets[i].y = playerY;
                        bullets[i].active = true;
                        break;
                    }
                }
            }

            // clicar no botão "Voltar"
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;
                if (mx > btnBack.x && mx < btnBack.x + btnBack.w &&
                    my > btnBack.y && my < btnBack.y + btnBack.h) {
                    running = false;
                }
            }
        }

        if (!gameOver) {
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            if (keystate[SDL_SCANCODE_LEFT] && playerX > 0)
                playerX -= playerSpeed;
            if (keystate[SDL_SCANCODE_RIGHT] && playerX < SCREEN_WIDTH - 50)
                playerX += playerSpeed;

            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    bullets[i].y -= bulletSpeed;
                    if (bullets[i].y < 0) bullets[i].active = false;
                }
            }

            bool edgeHit = false;
            for (int row = 0; row < ENEMY_ROWS; row++) {
                for (int col = 0; col < ENEMY_COLS; col++) {
                    if (enemies[row][col].alive) {
                        enemies[row][col].x += enemySpeedX * enemyDirection;
                        if (enemies[row][col].x <= 0 || enemies[row][col].x + enemyWidth >= SCREEN_WIDTH) {
                            edgeHit = true;
                        }
                    }
                }
            }
            if (edgeHit) {
                enemyDirection *= -1;
                for (int row = 0; row < ENEMY_ROWS; row++) {
                    for (int col = 0; col < ENEMY_COLS; col++) {
                        enemies[row][col].y += enemyHeight;
                    }
                }
            }

            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    SDL_Rect bulletRect = { bullets[i].x, bullets[i].y, 5, 10 };
                    for (int row = 0; row < ENEMY_ROWS; row++) {
                        for (int col = 0; col < ENEMY_COLS; col++) {
                            if (enemies[row][col].alive) {
                                SDL_Rect enemyRect = {
                                    enemies[row][col].x,
                                    enemies[row][col].y,
                                    enemyWidth,
                                    enemyHeight
                                };
                                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                                    enemies[row][col].alive = false;
                                    bullets[i].active = false;
                                    score += 10;
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // derrota, inimigo tocou no jogador
            for (int row = 0; row < ENEMY_ROWS && !gameOver; row++) {
                for (int col = 0; col < ENEMY_COLS && !gameOver; col++) {
                    if (enemies[row][col].alive && enemies[row][col].y + enemyHeight >= playerY) {
                        gameOver = true;
                        running = false;
                    }
                }
            }

            // vitória, todos eliminados
            bool allDead = true;
            for (int row = 0; row < ENEMY_ROWS; row++) {
                for (int col = 0; col < ENEMY_COLS; col++) {
                    if (enemies[row][col].alive) {
                        allDead = false;
                        break;
                    }
                }
            }
            if (allDead) {
                gameOver = true;
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // jogador
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect player = { playerX, playerY, 50, 20 };
        SDL_RenderFillRect(renderer, &player);

        // balas
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                SDL_Rect bullet = { bullets[i].x, bullets[i].y, 5, 10 };
                SDL_RenderFillRect(renderer, &bullet);
            }
        }

        // inimigos
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int row = 0; row < ENEMY_ROWS; row++) {
            for (int col = 0; col < ENEMY_COLS; col++) {
                if (enemies[row][col].alive) {
                    SDL_Rect enemy = {
                        enemies[row][col].x,
                        enemies[row][col].y,
                        enemyWidth,
                        enemyHeight
                    };
                    SDL_RenderFillRect(renderer, &enemy);
                }
            }
        }

        // botão "Voltar"
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &btnBack);
        SDL_Color white = {255, 255, 255};
        renderTextCentered(renderer, font, "Back", white, btnBack);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);

    return score;
}
