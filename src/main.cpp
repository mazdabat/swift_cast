#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <stdlib.h>

#include <filesystem>
#include <utile.hpp>
#include <vector>

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_surface.h"
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

    std::vector<SDL_Texture*> vec_textures;

    for (const auto& entry : std::filesystem::directory_iterator(resource_path)) {
        // extension name is not reliable - may need rely on the meatdata.
        if (entry.path().extension() == ".png") {
            SDL_Surface* surface = SDL_LoadPNG(entry.path().string().c_str());
            if (!surface) continue;

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_DestroySurface(surface);

            // texture is the slideshow
            if (texture) vec_textures.push_back(texture);
        }
    }

    Uint64 splash_switch = SDL_GetTicks();
    const Uint64 SPLASH_DELAY = 3000;
    bool display_splash = true;

    Uint64 lastSwitch = SDL_GetTicks();

    // Slide show
    size_t current = 0;
    const Uint64 IMAGE_DELAY = 5000;

    // Slide transitioning
    const Uint64 TRANSITION_TIME = 1000;
    bool transitioning = false;
    size_t next = 0;
    Uint64 transition_start = 0;

    int win_width, win_height;

    while (1) {
        Uint64 now = SDL_GetTicks();

        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT)  // Windows's close button
            break;

        // Scancodes physical key position (layout-independent)
        else if (event.type == SDL_EVENT_KEY_DOWN &&
                 event.key.scancode == SDL_SCANCODE_ESCAPE)  // Press ECS key down
            break;
        else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            win_width = win_width = event.window.data1;
            win_height = event.window.data2;
        }

        SDL_RenderClear(renderer);

        if (win_width <= 1 || win_height <= 1) continue;

        if (display_splash && win_width > 1 && win_height > 1) {
            display_splash = false;

            while (now - splash_switch <= 3000) {
                now = SDL_GetTicks();

                SDL_Texture* tex_splash =
                    load_svg_texture(renderer, win_width, win_height);

                SDL_RenderTexture(renderer, tex_splash, nullptr, nullptr);

                SDL_RenderPresent(renderer);
            }
        }

        if (!transitioning && (now - lastSwitch >= IMAGE_DELAY)) {
            // Change to the next slide, and play all slides in a loop
            // current = (current + 1) % vec_textures.size();
            // lastSwitch = now;

            // Trigger the transition
            transitioning = true;
            transition_start = now;
            next = (current + 1) % vec_textures.size();
        }

        // right to left tansition effect
        SDL_FRect cur_rect =
            GetFitRect(vec_textures[current], win_width, win_height);
        SDL_FRect next_rect =
            GetFitRect(vec_textures[next], win_width, win_height);

        if (transitioning) {
            float t = (now - transition_start) / (float)TRANSITION_TIME;
            if (t >= 1.0f) {
                current = next;
                transitioning = false;
                lastSwitch = now;
                t = 1.0f;
            }

            cur_rect.x = -t * win_width;
            next_rect.x = (1.0f - t) * win_width;

            SDL_SetTextureAlphaMod(vec_textures[current],
                (Uint8)((1.0f * t) * 255.0f));
            SDL_SetTextureAlphaMod(vec_textures[next], (Uint8)(t * 255.0f));

            SDL_RenderTexture(renderer, vec_textures[current], nullptr, &cur_rect);
            SDL_RenderTexture(renderer, vec_textures[next], nullptr, &next_rect);

        } else {
            // Stretched texture
            SDL_RenderTexture(renderer, vec_textures[current], nullptr, nullptr);
        }

        SDL_RenderPresent(renderer);
    }

    for (SDL_Texture* tex : vec_textures) {
        SDL_DestroyTexture(tex);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
