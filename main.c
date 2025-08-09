#define SDL_MAIN_HANDLED
#include "common.h"
#include "intro.h"
#include "menu.h"
#include "game.h"
#include "scores.h"
#include "render.h"
#include <string.h>

int main(int argc, char *argv[]) {
    //freopen("log.txt", "w", stdout);
    //printf("programa iniciado\n"); fflush(stdout);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        //fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    //printf("sdl inicializado\n"); fflush(stdout);

    if (TTF_Init() == -1) {
        //fprintf(stderr, "ttf_init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    //printf("ttf inicializado\n"); fflush(stdout);

    SDL_Window* window = SDL_CreateWindow("Space Invaders",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        //fprintf(stderr, "sdl_createWindow failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    //printf("janela criada\n"); fflush(stdout);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        //fprintf(stderr, "sdl_createRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    //printf("renderer criado\n"); fflush(stdout);

    TTF_Font* fontSmall  = TTF_OpenFont("cour.ttf", 20);
    TTF_Font* fontMedium = TTF_OpenFont("cour.ttf", 36);
    TTF_Font* fontLarge  = TTF_OpenFont("cour.ttf", 64);

    // introduçao
    introScreen(renderer, fontSmall);
    //printf("ecra de introdução mostrado\n"); fflush(stdout);

    char playerName[MAX_NAME] = "";

    while (1) {
        //printf("a mostrar menu\n"); fflush(stdout);
        int option = menuSDL(renderer, fontMedium, fontLarge);
        //printf("menusdl retornou: %d\n", option); fflush(stdout);

        if (option == 1) {
            //printf("opção Start Game escolhida\n"); fflush(stdout);
            //printf("inicializando input do nome do jogador\n"); fflush(stdout);

            bool enteringName = true;
            SDL_StartTextInput();
            SDL_Event event;
            playerName[0] = '\0';

            while (enteringName) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_Color white = {255, 255, 255};
                SDL_Color yellow = {255, 255, 0};

                renderText(renderer, fontMedium, "Enter your name:", white, 100, 150);
                renderText(renderer, fontMedium, playerName, yellow, 100, 220);

                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        //printf("evento: SDL_QUIT\n"); fflush(stdout);
                        enteringName = false;
                        option = 3;
                        break;
                    } else if (event.type == SDL_TEXTINPUT) {
                        //printf("evento: SDL_TEXTINPUT: %s\n", event.text.text); fflush(stdout);
                        if (strlen(playerName) + strlen(event.text.text) < MAX_NAME - 1) {
                            strcat(playerName, event.text.text);
                        }
                    } else if (event.type == SDL_KEYDOWN) {
                        //printf("evento: SDL_KEYDOWN: %d\n", event.key.keysym.sym); fflush(stdout);
                        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(playerName) > 0) {
                            playerName[strlen(playerName) - 1] = '\0';
                        } else if (event.key.keysym.sym == SDLK_RETURN && strlen(playerName) > 0) {
                            enteringName = false;
                        }
                    }
                }
            }

            SDL_StopTextInput();
            //printf("nome inserido: '%s'\n", playerName); fflush(stdout);

            // Inicia o jogo se o nome for válido
            if (strlen(playerName) > 0) {
                //printf("chamando playGame com nome: %s\n", playerName); fflush(stdout);
                int score = playGame(playerName, renderer);
                //printf("jogo terminado, score: %d\n", score); fflush(stdout);
                saveScore(playerName, score);
                //printf("pontuação guardada.\n"); fflush(stdout);
            }
        } else if (option == 2) {
            //printf("opção show scores escolhida\n"); fflush(stdout);
            showScores(renderer, fontMedium);
        } else {
            //printf("opção sair escolhida\n"); fflush(stdout);
            break;
        }
    }

    //printf("limpando e saindo\n"); fflush(stdout);
    TTF_CloseFont(fontSmall);
    TTF_CloseFont(fontMedium);
    TTF_CloseFont(fontLarge);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    //printf("fim\n"); fflush(stdout);

    return 0;
}
