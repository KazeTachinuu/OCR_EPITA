#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "pixel.h"

uint32_t pixeltograyscale(uint32_t pixel_color, SDL_PixelFormat* format)
{
    uint8_t r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    uint8_t average = (uint8_t) (0.3 * r + 0.59 * g + 0.11 * b);
    return SDL_MapRGB(format, average, average, average);
}

void surfacetograyscale(SDL_Surface* surface)
{
    uint32_t *pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    for (int i = 0; i < len; i++)
        pixels[i] = pixeltograyscale(pixels[i], format);
    SDL_UnlockSurface(surface);
}
