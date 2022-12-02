#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* grayscale)
{
    SDL_Event event;
    SDL_Texture* t = colored;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                return;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    draw(renderer, t);
                break;
            
            case SDL_KEYDOWN:
                t = t == colored ? grayscale : colored;
                draw(renderer, t);

        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* s = IMG_Load(path);
    return SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_RGB888, 0);
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = (Uint8) (0.3*r + 0.59*g + 0.11*b);
    return SDL_MapRGB(format, average, average, average);
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if(SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    for (int i = 0; i < len; i++)
        pixels[i] = pixel_to_grayscale(pixels[i], format);
    SDL_UnlockSurface(surface);
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // TODO:
    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("a", 0, 0, 0, 0, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // - Create a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);
    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w, surface->h);
    // - Create a texture from the colored surface.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    // - Convert the surface into grayscale.
    surface_to_grayscale(surface);
    // - Create a new texture from the grayscale surface.
    SDL_Texture* gtexture = SDL_CreateTextureFromSurface(renderer, surface);
    // - Free the surface.
    SDL_FreeSurface(surface);
    // - Dispatch the events.
    event_loop(renderer, texture, gtexture);
    // - Destroy the objects.
    SDL_DestroyTexture(gtexture);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}