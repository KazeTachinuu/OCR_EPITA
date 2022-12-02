#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pixel.h"

//THRESHOLD CALCULATION (mean - C)

#define AT_RANGE    5
#define AT_C        3

uint32_t AT_getThreshold(
    SDL_Surface *surface, int x, int y, int range)
{
    uint32_t mean = 0;
    uint32_t c = AT_C;
    double sum = 0;
    uint32_t pixelct = 0;

    for (int dy = -range; dy < range; dy++)
        if (y + dy >= 0 && y + dy < surface->h)
            for (int dx = -range; dx < range; dx++)
                if (x + dx >= 0 && x + dx < surface->w)
                {
                    sum += (double) (getPixel(surface, x+dx, y+dy) & 0xff);
                    pixelct++;
                }
    if (pixelct)
        mean = sum/pixelct;
    return mean > c ? mean - c : 0;
};

SDL_Surface *AdaptiveThresholding(SDL_Surface* surface)
{
    SDL_Surface *res = SDL_CreateRGBSurface(0,
                                surface->w,
                                surface->h,
                                surface->format->BitsPerPixel,
                                surface->format->Rmask,
                                surface->format->Gmask,
                                surface->format->Bmask,
                                surface->format->Amask);
    int range = AT_RANGE;
    for (int x = 0; x < surface->w; ++x)
    {
        for (int y = 0; y < surface->h; ++y)
        {
            uint32_t threshold = 0;
            threshold = AT_getThreshold(surface, y, x, range);
            uint32_t pixel_value = 0;
            pixel_value = getPixel(surface, x, y) & 0xff;
            if (pixel_value > threshold)
                setPixel(res, x, y, 0xFF000000);
            else
                setPixel(res, x, y, 0xFFFFFFFF); // Black pixel
        }
    }
    return res;
}
