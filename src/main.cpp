#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <stdlib.h>

#include <filesystem>
#include <vector>

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"

int main(int, char**) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Couldn't initialize SDL: %s",
            SDL_GetError());
        return 1;
    }  // Initialize SDL3

    if (!SDL_CreateWindowAndRenderer("Hello swift-cast",
            0,
            0,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN,
            &window,
            &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Couldn't create windows and renderer: %s",
            SDL_GetError());
    }

    // For old standers and GCCs, use boost in place of std
    std::filesystem::path resource_path =
        std::filesystem::current_path() / std::filesystem::path("res");
    resource_path = std::filesystem::absolute(resource_path);
    SDL_Log("Current resource path: %s", resource_path.u8string().c_str());

    std::vector<SDL_Texture*> vector_textures;

    for (const auto& entry : std::filesystem::directory_iterator(resource_path)) {
        // extension name is not reliable - may need rely on the meatdata.
        if (entry.path().extension() == ".png") {
            SDL_Surface* surface = SDL_LoadPNG(entry.path().string().c_str());
            if (!surface) continue;

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_DestroySurface(surface);

            // texture is the slideshow
            if (texture) vector_textures.push_back(texture);
        }
    }

    size_t current = 0;
    Uint64 lastSwitch = SDL_GetTicks();
    const Uint64 IMAGE_DELAY = 2000;

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT)  // Windows's close button
            break;

        else if (event.type == SDL_EVENT_KEY_DOWN)  // Any key down
            break;

        Uint64 now = SDL_GetTicks();
        if (now - lastSwitch >= IMAGE_DELAY) {
            // Play the slides in a loop
            current = (current + 1) % vector_textures.size();
            lastSwitch = now;
        }

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, vector_textures[current], NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
