#include <SDL3/SDL.h>

SDL_FRect GetFitRect(SDL_Texture* texture_param, int window_width,
    int window_height);

SDL_Texture* load_svg_texture(SDL_Renderer* renderer, int target_w, int target_h);