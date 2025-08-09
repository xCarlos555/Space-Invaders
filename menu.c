#include "menu.h"
#include "render.h"

int menuSDL(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *titleFont) {
    SDL_Rect btnStart  = { SCREEN_WIDTH/2 - 150, 200, 300, 60 };
    SDL_Rect btnScores = { SCREEN_WIDTH/2 - 150, 300, 300, 60 };
    SDL_Rect btnExit   = { SCREEN_WIDTH/2 - 150, 400, 300, 60 };

    bool running = true;
    int option = 0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                option = 3; // sair
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                // botão Start
                if (mx > btnStart.x && mx < btnStart.x + btnStart.w &&
                    my > btnStart.y && my < btnStart.y + btnStart.h) {
                    option = 1;
                    running = false;
                }
                // botão Scores
                else if (mx > btnScores.x && mx < btnScores.x + btnScores.w &&
                         my > btnScores.y && my < btnScores.y + btnScores.h) {
                    option = 2;
                    running = false;
                }
                // botão Exit
                else if (mx > btnExit.x && mx < btnExit.x + btnExit.w &&
                         my > btnExit.y && my < btnExit.y + btnExit.h) {
                    option = 3;
                    running = false;
                }
            }
        }

        // verifica posição do rato para hover
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // fundo
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // título centralizado
        SDL_Color titleColor = {255, 255, 0};
        int titleW, titleH;
        TTF_SizeText(titleFont, "SPACE INVADERS", &titleW, &titleH);
        int titleX = (SCREEN_WIDTH - titleW) / 2;
        renderText(renderer, titleFont, "SPACE INVADERS", titleColor, titleX, 80);

        SDL_SetRenderDrawColor(renderer, 0, (mx > btnStart.x && mx < btnStart.x + btnStart.w && my > btnStart.y && my < btnStart.y + btnStart.h) ? 255 : 200, 0, 255);
        SDL_RenderFillRect(renderer, &btnStart);

        SDL_SetRenderDrawColor(renderer, 0, (mx > btnScores.x && mx < btnScores.x + btnScores.w && my > btnScores.y && my < btnScores.y + btnScores.h) ? 255 : 200, 255, 255);
        SDL_RenderFillRect(renderer, &btnScores);

        SDL_SetRenderDrawColor(renderer, (mx > btnExit.x && mx < btnExit.x + btnExit.w && my > btnExit.y && my < btnExit.y + btnExit.h) ? 255 : 200, 0, 0, 255);
        SDL_RenderFillRect(renderer, &btnExit);

        SDL_Color white = {255, 255, 255};
        renderTextCentered(renderer, font, "Start Game", white, btnStart);
        renderTextCentered(renderer, font, "High Scores", white, btnScores);
        renderTextCentered(renderer, font, "Exit", white, btnExit);

        SDL_RenderPresent(renderer);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return option;
}
