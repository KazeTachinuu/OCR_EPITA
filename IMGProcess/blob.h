#ifndef BLOB_H
#define BLOB_H

typedef struct
{
    SDL_Surface* res;
    int x;
    int y;
    size_t size;
} Blob;

size_t floodfill(SDL_Surface *surface, int x, int y, uint32_t oldcolor, uint32_t newcolor);
Blob findbiggestblob(SDL_Surface *surface);
void removeblobs(SDL_Surface *surface, size_t threshold, uint32_t color1, uint32_t bgcolor);

#endif
