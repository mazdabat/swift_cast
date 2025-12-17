#include <utile.hpp>

#include "SDL3/SDL_render.h"

SDL_FRect GetFitRect(SDL_Texture* texture_param, int win_width, int win_height) {
    float texture_width, texture_height;
    SDL_GetTextureSize(texture_param, &texture_width, &texture_height);

    float texAspect = texture_width / texture_height;
    float winAspect = (float)win_width / (float)win_height;

    SDL_FRect dst{};

    dst.x = 0;
    dst.y = 0;

    if (winAspect > texAspect) {
        // Window is wider → scale based on height
        dst.h = (float)win_height;
        dst.w = (float)win_height * texAspect;
    } else {
        // Window is taller → scale based on width
        dst.w = (float)win_width;
        dst.h = (float)win_width / texAspect;
    }

    return dst;
}
