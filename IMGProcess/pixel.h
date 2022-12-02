#ifndef PIXEL_H
#define PIXEL_H

uint32_t getPixel(SDL_Surface *surface, int x, int y);
void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int I(SDL_Surface *source, int x, int y);
void drawLine(SDL_Surface *surface, int x0, int y0, int x1, int y1, uint32_t p);

#endif
