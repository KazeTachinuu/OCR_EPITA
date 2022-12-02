#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

#include "pixel.h"
#include "queue.h"
#include "utils.h"
#include "blob.h"

size_t floodfill(SDL_Surface *surface, int x, int y, uint32_t oldcolor, uint32_t newcolor)
{
    struct PointQueue *q = malloc(sizeof(struct PointQueue));
    q->x = 0;
    q->y = 0;
    q->next = NULL;

    enqueue(q, x, y);
    size_t nbpoint = 0;
    while (!(isempty(q)))
    {
        struct PointQueue *curr = dequeue(q);
        int cx = curr->x;
        int cy = curr->y;
        free(curr);

        if (cx < 0 || cx >= surface->w || cy < 0 || cy >= surface->h)
            continue;
        if (getPixel(surface, cx, cy) == oldcolor)
        {
            setPixel(surface, cx, cy, newcolor);
            nbpoint++;

            enqueue(q, cx - 1, cy);
            enqueue(q, cx + 1, cy);
            enqueue(q, cx, cy - 1);
            enqueue(q, cx, cy + 1);
        }
    }
    return nbpoint;
}

Blob findbiggestblob(SDL_Surface *surface)
{
    SDL_Surface *ret = SDL_CreateRGBSurface(0,
            src->w,
            src->h,
            src->format->BitsPerPixel,
            src->format->Rmask,
            src->format->Gmask,
            src->format->Bmask,
            src->format->Amask);
    SDL_BlitSurface(surface, NULL, ret, NULL);

    size_t max = 0;
    int max_x;
    int max_y;

    for (int y = 0; y < surface->h; y++)
        for (int x = 0; x < surface->w; x++)
        {
            if (getPixel(ret, x, y) == 0xFF000000)
            {
                size_t area = floodfill(ret, x, y, 0xFF000000, 0xFF0000FF);
                if (area > max)
                {
                    max = area;
                    max_x = x;
                    max_y = y;
                }
            }
        }
    floodfill(ret, max_x, max_y, 0xFF0000FF, 0xFF000000);

    for (int y = 0; y < src->h; y++)
    {
        for (int x = 0; x < src->w; x++)
        {
            if (x != max_x && y != max_y && getPixel(ret, x, y) == 0xFF0000FF)
                floodfill(ret, x, y, 0xFF0000FF, 0xFF000000);
        }
    }

    return (Blob) {dest, max_x, max_y, max};
}

void removeblobs(SDL_Surface *surface, size_t threshold, uint32_t color1, uint32_t bgcolor)
{
    if (color1 == 0xFF0000FF)
        errx(1, "skill issue");
    for (int y = 0; y < surface->h; y++)
        for (int x = 0; x < surface->w; x++)
        {
            if (getPixel(surface, x, y) != color1) continue;
            size_t blobsize = floodfill(surface, x, y, color1, 0xFF0000FF);
            if (blobsize <= threshold)
            {
                floodfill(surface, x, y, 0xFF0000FF, bgcolor);
            }
        }

    for (int y = 0; y < surface->h; y++)
        for (int x = 0; x < surface->w; x++)
            if (getPixel(surface, x, y) == 0xFF0000FF)
                floodfill(surface, x, y, 0xFF0000FF, color1);
}
