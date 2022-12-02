#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "pixel.h"

SDL_Surface *rotate_shearing(SDL_Surface *surface, double angle)
{
    int w = surface->w;
    int h = surface->h;

    SDL_Surface *rotimage = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, 0);
    double r_angle = angle * (M_PI / 180); //should we angle mod 360 for prec?
    int mid_x = w / 2;
    int mid_y = h / 2;
    double sin_angle = sin(r_angle);
    double tan_angle = tan(r_angle/2);
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
        {
            double dx = (x - mid_x);
            double dy = (y - mid_y);

            int new_x = round(dx - dy * tan_angle);
            int new_y = dy;

            new_y = round(new_x * sin_angle + new_y);
            new_x = round(new_x - new_y * tan_angle);

            new_x += mid_x;
            new_y += mid_y;
            if (new_x >= 0 && new_x < w && new_y >= 0 && new_y < h)
            {
                uint32_t pixel = getPixel(surface, x, y);
                setPixel(rotimage, new_x, new_y, pixel);
            }
        }
    return rotimage;
}
