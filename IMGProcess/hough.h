#ifndef HOUGH_H
#define HOUGH_H

int houghtransform(SDL_Surface *s, double** rho_array, double** theta_array);

void drawlines(SDL_Surface *s, int nblines, double** rho_array, double** theta_array);

#endif
