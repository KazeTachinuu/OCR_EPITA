#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "pixel.h"

uint32_t getPixel(SDL_Surface *surface, int x, int y)
{
    //stackoverflow "how to get pixel sdl_image"
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        //error
    }
    uint32_t result =
        *(uint32_t *) ((uint8_t *) surface->pixels + y * surface->pitch
                + x * surface->format->BytesPerPixel);
    return result;
}

void setPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(uint16_t *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

int I(SDL_Surface *source, int x, int y)
{
    return getPixel(source, x, y) & 0xFF;
}

void drawLine(SDL_Surface *surface, int x0, int y0, int x1, int y1, uint32_t p)
{
    double length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    double dx = (x1 - x0) / length;
    double dy = (y1 - y0) / length;
    double x = x0;
    double y = y0;
    for (int i = 0; i < length; i++)
    {
        setPixel(surface, x, y, p);
        x += dx;
        y+= dy;
    }
}
