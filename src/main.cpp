#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <stdlib.h>

#include <filesystem>

#define WIDTH 1280
#define HEIGHT 720

int main(int, char**) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }  // Initialize SDL3

    if (!SDL_CreateWindowAndRenderer("Hello swift-cast", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN,
                                     &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create windows and renderer: %s", SDL_GetError());
    }

    std::filesystem::path current_path = std::filesystem::current_path();
    SDL_Log("Current path as string: %s", current_path.u8string().c_str());

    surface = SDL_LoadPNG("res/pic_sc.png");
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    }

    SDL_DestroySurface(surface);

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
            break;
        else if (event.type == SDL_EVENT_KEY_DOWN)
            break;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
