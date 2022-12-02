#ifndef MONOCHROME_H
#define MONOCHROME_H

uint32_t AT_getThreshold(SDL_Surface *surface, int x, int y, int range);
SDL_Surface *AdaptiveThresholding(SDL_Surface* surface);

#endif
