#ifndef GRAYSCALE_H
#define GRAYSCALE_H

uint32_t pixeltograyscale(uint32_t pixel_color, SDL_PixelFormat* format);
void surfacetograyscale(SDL_Surface* surface);

#endif
