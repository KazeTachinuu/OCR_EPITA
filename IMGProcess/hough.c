#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

#include "pixel.h"
#include "hough.h"
#include "utils.h"

#ifndef M_PI
#define M_PI 3.1415927
#endif

int houghtransform(SDL_Surface* s, double** rho_array, double** theta_array)
{
    int ntheta = 180;
    int nrho = sqrt(s->h * s->h + s->w * s->w);

    double dtheta = M_PI / (double) ntheta;
    double drho = sqrt(s->h * s->h + s->w * s->w) / nrho;

    long* acc = calloc(ntheta * nrho, sizeof(long*));
    if (acc == NULL)
        errx(1, "noooo :sob:");

    for (int y = 0; y < s->h; y++)
    {
        for (int x = 0; x < s->w; x++)
        {
            int intensity = getPixel(s, x, y) & 0xff;

            if (intensity == 0xff) //white
            {
                for (int itheta = 0; itheta < ntheta; itheta++)
                {
                    int theta = itheta * dtheta;
                    int irho = x * cos(theta) + (s->h - y) * sin(theta)
                        / drho;
                    if (irho > 100 && irho < nrho)
                        acc[itheta * nrho + irho]++;
                }
            }
        }
    }

    int threshold = 600; //highest values
    int nblines = 0;
    for (int i = 0; i < ntheta * nrho; i++)
    {
        if (acc[i] < threshold)
            acc[i] = 0;
        else
            nblines++;
    }

    *rho_array = calloc(nblines, sizeof(double));
    *theta_array = calloc(nblines, sizeof(double));
    int index = 0;
    for (int itheta = 0; itheta < ntheta; itheta++)
        for (int irho = 0; irho < nrho; irho++)
        {
            if (acc[itheta * nrho + irho])
            {
                *rho_array[index] = irho * drho;
                *rho_array[index++] = itheta * dtheta;
            }
        }
    free(acc);
    return nblines;
}

void drawlines(SDL_Surface *s, int nblines, double** rho_array, double ** theta_array)
{

    for (int i = 0; i < nblines; i++)
    {
        double rho = *rho_array[i];
        double theta = *theta_array[i];

        printf("Lines with rho=%f and theta=%f\n", rho, theta);

        double a = cos(theta);
        double b = sin(theta);

        int x0 = floor(a * rho);
        int y0 = floor(b * rho);
        int x1 = clamp(floor(x0 + 1000 * (-b)), 0, s->w - 1);
        int y1 = clamp(floor(y0 + 1000 * a)   , 0, s->h - 1);
        int x2 = clamp(floor(x0 - 1000 * (-b)), 0, s->h - 1);
        int y2 = clamp(floor(y0 - 1000 * a)   , 0, s->h - 1);

        drawLine(s, x1, y1, x2, y2, 0xFF00FFFF);
    }
}
