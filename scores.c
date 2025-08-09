#include "scores.h"
#include "render.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[MAX_NAME];
    int score;
} ScoreEntry;

int compareScores(const void *a, const void *b) {
    const ScoreEntry *sa = (const ScoreEntry *)a;
    const ScoreEntry *sb = (const ScoreEntry *)b;
    return sb->score - sa->score;
}

void askPlayerName(SDL_Renderer *renderer, TTF_Font *font, char *nameOut) {
    SDL_StartTextInput();
    SDL_Event event;
    bool typing = true;
    char input[MAX_NAME] = "";
    int len = 0;

    while (typing) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color yellow = {255, 255, 0};
        SDL_Color white = {255, 255, 255};

        renderText(renderer, font, "Enter your name:", yellow, SCREEN_WIDTH / 2 - 150, 200);

        SDL_Rect inputRect = {SCREEN_WIDTH / 2 - 150, 280, 300, 50};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &inputRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &inputRect);
        renderText(renderer, font, input, white, inputRect.x + 10, inputRect.y + 10);

        SDL_Rect btnOK = {SCREEN_WIDTH / 2 - 75, 380, 150, 50};
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_RenderFillRect(renderer, &btnOK);
        renderTextCentered(renderer, font, "Start", white, btnOK);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                typing = false;
                strcpy(input, "Player");
            } else if (event.type == SDL_TEXTINPUT) {
                if (len + strlen(event.text.text) < MAX_NAME - 1) {
                    strcat(input, event.text.text);
                    len += strlen(event.text.text);
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && len > 0) {
                    input[--len] = '\0';
                } else if (event.key.keysym.sym == SDLK_RETURN && len > 0) {
                    typing = false;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;
                if (mx > btnOK.x && mx < btnOK.x + btnOK.w &&
                    my > btnOK.y && my < btnOK.y + btnOK.h &&
                    len > 0) {
                    typing = false;
                }
            }
        }
    }

    SDL_StopTextInput();
    strncpy(nameOut, input, MAX_NAME);
}


void saveScore(const char *name, int score) {
    ScoreEntry scores[100];
    int count = 0;
    bool updated = false;

    FILE *f = fopen(SCORES_FILE, "r");
    if (f) {
        while (fscanf(f, "%49s %d", scores[count].name, &scores[count].score) == 2) {
            if (strcmp(scores[count].name, name) == 0) {
                if (score > scores[count].score) {
                    scores[count].score = score;
                }
                updated = true;
            }
            count++;
        }
        fclose(f);
    }

    if (!updated) {
        strncpy(scores[count].name, name, MAX_NAME);
        scores[count].score = score;
        count++;
    }

    f = fopen(SCORES_FILE, "w");
    if (!f) return;

    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %d\n", scores[i].name, scores[i].score);
    }

    fclose(f);
}

void clearScores() {
    FILE *f = fopen(SCORES_FILE, "w");
    if (f) fclose(f);
}

void showScores(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Event event;
    bool waiting = true;

    SDL_Rect btnBack  = { SCREEN_WIDTH/2 - 170, 500, 150, 60 };
    SDL_Rect btnClear = { SCREEN_WIDTH/2 + 20, 500, 150, 60 };

    ScoreEntry scores[100];
    int count = 0;
    FILE *f = fopen(SCORES_FILE, "r");
    if (f) {
        while (fscanf(f, "%49s %d", scores[count].name, &scores[count].score) == 2) {
            count++;
        }
        fclose(f);
    }

    qsort(scores, count, sizeof(ScoreEntry), compareScores);

    while (waiting) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color yellow = {255, 255, 0};
        SDL_Color white = {255, 255, 255};

        int titleW, titleH;
        TTF_SizeText(font, "HIGH SCORES", &titleW, &titleH);
        renderText(renderer, font, "HIGH SCORES", yellow,
                   (SCREEN_WIDTH - titleW) / 2, 50);

        int y = 120;
        for (int i = 0; i < count; i++) {
            char line[100];
            snprintf(line, sizeof(line), "%s - %d", scores[i].name, scores[i].score);
            renderText(renderer, font, line, yellow, 100, y);
            y += 40;
        }
        if (count == 0) {
            renderText(renderer, font, "No scores yet!", yellow, 100, 120);
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &btnBack);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &btnClear);

        renderTextCentered(renderer, font, "Back", white, btnBack);
        renderTextCentered(renderer, font, "Clear", white, btnClear);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                waiting = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                if (mx > btnBack.x && mx < btnBack.x + btnBack.w &&
                    my > btnBack.y && my < btnBack.y + btnBack.h) {
                    waiting = false;
                }

                if (mx > btnClear.x && mx < btnClear.x + btnClear.w &&
                    my > btnClear.y && my < btnClear.y + btnClear.h) {

                    bool confirming = true;
                    while (confirming) {
                        SDL_Event confirmEvent;

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
                        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                        SDL_RenderFillRect(renderer, &overlay);

                        SDL_Rect box = { SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 100, 400, 200 };
                        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                        SDL_RenderFillRect(renderer, &box);

                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderDrawRect(renderer, &box);

                        renderTextCentered(renderer, font, "Clear all scores?", white, (SDL_Rect){box.x, box.y + 30, box.w, 40});

                        SDL_Rect btnYes = { box.x + 40, box.y + 120, 120, 50 };
                        SDL_Rect btnNo  = { box.x + 240, box.y + 120, 120, 50 };

                        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                        SDL_RenderFillRect(renderer, &btnYes);
                        renderTextCentered(renderer, font, "Yes", white, btnYes);

                        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &btnNo);
                        renderTextCentered(renderer, font, "No", white, btnNo);

                        SDL_RenderPresent(renderer);

                        while (SDL_PollEvent(&confirmEvent)) {
                            if (confirmEvent.type == SDL_QUIT) {
                                confirming = false;
                                waiting = false;
                            } else if (confirmEvent.type == SDL_MOUSEBUTTONDOWN) {
                                int cx = confirmEvent.button.x;
                                int cy = confirmEvent.button.y;

                                if (cx > btnYes.x && cx < btnYes.x + btnYes.w &&
                                    cy > btnYes.y && cy < btnYes.y + btnYes.h) {
                                    clearScores();
                                    count = 0;
                                    confirming = false;
                                }

                                if (cx > btnNo.x && cx < btnNo.x + btnNo.w &&
                                    cy > btnNo.y && cy < btnNo.y + btnNo.h) {
                                    confirming = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
