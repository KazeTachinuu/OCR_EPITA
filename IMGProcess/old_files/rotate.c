#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pixel.h"

SDL_Surface *rotate(SDL_Surface *image, double angle) {
  int width = image->w;
  int height = image->h;

  SDL_Surface *r_image;
  r_image = SDL_CreateRGBSurfaceWithFormat(0, image->w, image->h,
                                           image->format->BitsPerPixel,
                                           image->format->format);

  double r_angle = angle * (M_PI / 180); // define constant
  int mid_x = width / 2;
  int mid_y = height / 2;
  double cos_angle = cos(r_angle);
  double sin_angle = sin(r_angle);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      double dx = (x - mid_x);
      double dy = (y - mid_y);
      int r_x = round(dx * cos_angle + dy * sin_angle + mid_x);
      int r_y = round(dy * cos_angle - dx * sin_angle + mid_y);
      if (0 <= r_x && r_x < width && 0 <= r_y && r_y < height) {
        Uint32 pixel = getPixel(image, x, y);
        setPixel(r_image, r_x, r_y, pixel);
      }
    }
  }
  SDL_FreeSurface(image);
  return r_image;
}


void saveImage(SDL_Surface *image, char *filename) {
  SDL_SaveBMP(image, filename);
  SDL_FreeSurface(image);
}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <image file>", argv[0]);
    return 1;
  }
  // if left arrow key is pressed, rotate left
  // if right arrow key is pressed, rotate right

  SDL_Init(SDL_INIT_VIDEO);
  //Get window surface
  int width = 484*2;
  int height = 416*2;
  SDL_Window *window =
      SDL_CreateWindow("Rotate", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, width, height, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Surface *image = IMG_Load(argv[1]);
  SDL_Surface *rotimg = IMG_Load(argv[1]); //safe
  double curr_angle;
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_Event event;
  int quit = 0;

  printf("Press left or right arrow key to rotate image\n");
  printf("Press return to save and quit\n");
  printf("Press escape to quit without saving\n\n");
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
      if (event.type == SDL_KEYDOWN) {
        //Destroy old texture
        SDL_DestroyTexture(texture);
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          curr_angle += 5;
          rotimage = rotate(image, curr_angle);
          texture = SDL_CreateTextureFromSurface(renderer, rotimage);
          break;
        case SDLK_RIGHT:
          curr_angle -= 5;
          rotimage = rotate(image, curr_angle);
          texture = SDL_CreateTextureFromSurface(renderer, rotimage);
          break;

        case SDLK_RETURN:
          strcat(argv[1], "_rotated.bmp");
          saveImage(rotimage, argv[1]);
          //quit program
          //print in green
          printf("\033[0;32m");
          printf("Image saved as %s\n", argv[1]);
          printf("\033[0m");
          return EXIT_SUCCESS;

        case SDLK_ESCAPE:
          //quit program
          //print EXIT SUCCESS in color
          printf("\033[0;32m");
          printf("Image not saved\n");
          printf("\033[0m");
          return EXIT_SUCCESS;
        }
      }
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(rotimage);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
