#include <SDL3/SDL_pixels.h>

#include <utile.hpp>

#include "SDL3/SDL_render.h"

#define NANOSVG_IMPLEMENTATION
#include <nanosvg.h>
#define NANOSVGRAST_IMPLEMENTATION
#include <nanosvgrast.h>
#include <sc_splash.h>

#include <vector>

/*
REMAINDER: Convert fonts to path when drawing SVG
*/

SDL_Texture* load_svg_texture(SDL_Renderer* renderer, int target_w, int target_h) {
    std::vector<char> svg_buffer(reinterpret_cast<char*>(__sc_logo_text_svg),
        reinterpret_cast<char*>(__sc_logo_text_svg) + __sc_logo_text_svg_len);
    svg_buffer.push_back('\0');

    NSVGimage* image = nsvgParse(svg_buffer.data(), "px", 96.0f);
    if (!image) return nullptr;

    const float scale = 5.0f;

    const int svg_w = (int)(image->width * scale);
    const int svg_h = (int)(image->height * scale);

    const int offset_x = (target_w - svg_w) / 2;
    const int offset_y = (target_h - svg_h) / 2;

    /*
    SVG buffer (transparent)
    - Represent 2D array in 1D array with size of width X height
    - 1D array size * 4 because RGBA has 4 bytes per pixel
    */
    std::vector<unsigned char> svg(svg_w * svg_h * 4, 0);

    NSVGrasterizer* rast = nsvgCreateRasterizer();
    nsvgRasterize(rast,
        image,
        0.0f,
        0.0f,
        scale,
        svg.data(),
        svg_w,
        svg_h,
        svg_w * 4);

    // White background buffer with target size
    std::vector<unsigned char> bg(target_w * target_h * 4);
    for (size_t i = 0; i < bg.size(); i += 4) {
        bg[i + 0] = 255;
        bg[i + 1] = 255;
        bg[i + 2] = 255;
        bg[i + 3] = 255;
    }

    // Alpha blend SVG into background on the
    for (int y = 0; y < svg_h; ++y) {
        for (int x = 0; x < svg_w; ++x) {
            // ptr_dst points to the texture data segment in bg buffer
            unsigned char* ptr_dst =
                &bg[((y + offset_y) * target_w + (x + offset_x)) * 4];

            const unsigned char* origin = &svg[(y * svg_w + x) * 4];

            // Blending formular for (R G B) - source over destination
            if (origin[3] != 0) {
                float a = origin[3] / 255.0f;
                ptr_dst[0] =
                    (unsigned char)(origin[0] * a + ptr_dst[0] * (1.0f - a));
                ptr_dst[1] =
                    (unsigned char)(origin[1] * a + ptr_dst[1] * (1.0f - a));
                ptr_dst[2] =
                    (unsigned char)(origin[2] * a + ptr_dst[2] * (1.0f - a));
                ptr_dst[3] = 255;
            }
        }
    }

    SDL_Surface* surface = SDL_CreateSurfaceFrom(target_w,
        target_h,
        SDL_PIXELFORMAT_RGBA32,
        bg.data(),
        target_w * 4);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);
    nsvgDeleteRasterizer(rast);
    nsvgDelete(image);

    return texture;
}

SDL_FRect GetFitRect(SDL_Texture* texture_param, int win_width, int win_height) {
    float texture_width, texture_height;
    SDL_GetTextureSize(texture_param, &texture_width, &texture_height);

    float texAspect = texture_width / texture_height;
    float winAspect = (float)win_width / (float)win_height;

    SDL_FRect dst{};

    dst.x = 0;
    dst.y = 0;

    if (winAspect > texAspect) {
        // Window is wider scale based on height
        dst.h = (float)win_height;
        dst.w = (float)win_height * texAspect;
    } else {
        // Window is taller scale based on width
        dst.w = (float)win_width;
        dst.h = (float)win_width / texAspect;
    }

    return dst;
}
