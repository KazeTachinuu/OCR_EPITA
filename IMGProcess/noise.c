#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

#include "pixel.h"
#include "new_grayscale.h"

#define G_RANGE  2


double gauss(double x, double sigma)
{
    //might need to define variables to avoid precision loss
    return exp(-(x*x) / (2 * sigma * sigma));
}

double* build_kernelv1(int size)
{
    double sum = 0;
    double *kernel = calloc(size, sizeof(double*));
    for (int i = 0; i < size; i++)
    {
        double val = gauss(i - G_RANGE, 1.3); //fixed values
        kernel[i] = val;
        sum += val;
    }

    for (int i = 0; i < size; i++) //normalize kernel
        kernel[i] /= sum;

    return kernel;
}

SDL_Surface* gaussian_blur(SDL_Surface* surface)
{
    SDL_Surface* res = SDL_CreateRGBSurface(0, surface->w, surface->h,
                                            surface->format->BitsPerPixel,
                                            surface->format->Rmask,
                                            surface->format->Gmask,
                                            surface->format->Bmask,
                                            surface->format->Amask);
    int size = 2 * G_RANGE + 1; //fixed value, (2*range +1)
    double* kernel = build_kernelv1(size);

    //aled

    unsigned char* aled = calloc(surface->w * surface->h, sizeof(size_t));
    for (int y = 0; y < surface-> h; y++)
    {
        for (int x = G_RANGE; x < surface->w - G_RANGE; x++)
        {
            double sum = 0;
            for (int w = 0; w < size; w++)
                sum += I(surface, x + w - G_RANGE, y) * kernel[w];
        aled[y * surface->w + x] = sum < 0 ? 0 : sum > 255 ? 255 : sum;
        }
    }

    for (int y = G_RANGE; y < surface->h - G_RANGE; y++)
    {
        for (int x = G_RANGE; x < surface->w - G_RANGE; x++)
        {
            double sum = 0;
            for (int w = 0; w < size; w++)
                sum += aled[(y + w - G_RANGE) * surface->w + x] * kernel[w];
            if (sum < 0)
                sum = 0;
            else if (sum > 255)
                sum = 255;
            setPixel(res, x, y, SDL_MapRGB(surface->format, sum, sum, sum));
        }
    }
    free(kernel);
    free(aled); //Psalm 10:17
    return res;
}

void gb_in_place(SDL_Surface *source)
{
    SDL_Surface *res = gaussian_blur(source);
    for (int i = G_RANGE; i < source->w - G_RANGE; i++)
    {
        for (int j = G_RANGE; j < source->h - G_RANGE; j++)
        {
            uint32_t pixel = getPixel(res, i, j);
            setPixel(source, i, j, pixel);
        }
    }
    SDL_FreeSurface(res);
}
