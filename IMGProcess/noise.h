#ifndef NOISE_H
#define NOISE_H

double gauss(double x, double sigma);
double* build_kernelv1(int size);
SDL_Surface *gaussian_blur(SDL_Surface* surface);
void gb_in_place(SDL_Surface *source);

#endif
