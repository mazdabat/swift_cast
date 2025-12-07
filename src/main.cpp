#include <SDL3/SDL.h>

int main(int, char**) {
    SDL_Log("%s", "Normal log");
    SDL_LogError(1, "Error log");
    SDL_LogCritical(4, "Critical log");
}
