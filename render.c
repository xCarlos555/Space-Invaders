#include "render.h"

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y) {
    if (!font || !text) {
        //printf("erro na fonte ou texto nulo em renderText\n");
        return;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        //printf("erro ao criar surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        //printf("erro ao criar textura: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderTextCentered(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect rect) {
    if (!font || !text) {
        //printf("erro: fonte ou texto nulo em renderTextCentered\n");
        return;
    }

    int textW, textH;
    if (TTF_SizeText(font, text, &textW, &textH) != 0) {
        //printf("erro ao medir texto: %s\n", TTF_GetError());
        return;
    }

    int x = rect.x + (rect.w - textW) / 2;
    int y = rect.y + (rect.h - textH) / 2;

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        //printf("erro ao criar surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        //printf("erro ao criar textura: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
