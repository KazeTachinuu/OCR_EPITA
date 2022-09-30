#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 999
#define WINDOW_HEIGHT (WINDOW_WIDTH)
#define GRIDMAX (WINDOW_WIDTH / 3)

#define SUDBOXMAX 3
#define SUDXMAX (SUDBOXMAX * 3)
#define SUDYMAX (SUDBOXMAX * 3)
typedef int board_t[SUDYMAX][SUDXMAX];

typedef unsigned char byte;

typedef struct {
  byte r;
  byte g;
  byte b;
#ifdef PIX32
  byte a;
#endif
} pixel_t;

board_t brd;


/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
                       TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
  int text_width;
  int text_height;
  SDL_Surface *surface;

  // Text color
  SDL_Color textColor = {219, 84, 97, 0};

  surface = TTF_RenderText_Solid(font, text, textColor);
  *texture = SDL_CreateTextureFromSurface(renderer, surface);

  text_width = surface->w;
  text_height = surface->h;

  SDL_FreeSurface(surface);

  rect->x = x;
  rect->y = y;
  rect->w = text_width;
  rect->h = text_height;
}


// prtboard -- print sudoku board as text
void prtboard(board_t brd,char* filename) {

  FILE *fp = fopen(filename, "w");
  for (int y = 0; y < SUDYMAX; ++y) {
    if ((y % 3) == 0 && y != 0)
      fprintf(fp,"\n");
    for (int x = 0; x < SUDYMAX; ++x) {
      if ((x % 3) == 0 && x != 0)
        fprintf(fp," ");
      int val = brd[y][x];
      if (val == 0)
        fprintf(fp,".");
      else
        fprintf(fp,"%d", val);
    }
    fprintf(fp,"\n");
  }
}

void drawgrid(SDL_Renderer *renderer) {
  int gridmax;
  int boxmax;
  SDL_Rect rect;

  // outer grid
  SDL_SetRenderDrawColor(renderer, 67, 58, 63, 0);

  gridmax = GRIDMAX;
  boxmax = SUDYMAX;
  for (int ybox = 0; ybox < boxmax; ++ybox) {
    int ybase = ybox * gridmax;
    rect.y = ybase;
    rect.h = gridmax;
    for (int xbox = 0; xbox < boxmax; ++xbox) {
      int xbase = xbox * gridmax;
      rect.x = xbase;
      rect.w = gridmax;
      SDL_RenderDrawRect(renderer, &rect);
    }
  }
}

void drawbox(SDL_Renderer *renderer) {
  int gridmax;
  int boxmax;
  SDL_Rect rect;

  // outer grid
  SDL_SetRenderDrawColor(renderer, 135, 137, 192, 0);

  gridmax = GRIDMAX / 3;
  boxmax = SUDYMAX * 3;
  for (int ybox = 0; ybox < boxmax; ++ybox) {
    int ybase = ybox * gridmax;
    rect.y = ybase;
    rect.h = gridmax;
    for (int xbox = 0; xbox < boxmax; ++xbox) {
      int xbase = xbox * gridmax;
      rect.x = xbase;
      rect.w = gridmax;
      SDL_RenderDrawRect(renderer, &rect);
    }
  }
}



void readboard(board_t board,char *filename) {
  int i, j;
  int counti = 0, countj = 0;
  FILE *infile;
  infile = fopen(filename, "r");
  for (i = 0; i < 9; i++) {
    countj = 0;
    if (counti == 3 || counti == 6) {
      fscanf(infile, "\n");
    }
    for (j = 0; j < 9; j++) {

      if (countj == 3 || countj == 6) {
        fscanf(infile, "%*c");
      }
      fscanf(infile, "%c", &board[i][j]);
      countj++;
    }
    fscanf(infile, "\n");
    counti++;
  }
  fclose(infile);
}



void drawtext(SDL_Renderer *renderer) {
  int gridmax;
  int boxmax_y;
  int boxmax_x;
  SDL_Rect grect;
  SDL_Rect trect;
  SDL_Texture *texture;
  char buf[2];

  // text
  SDL_SetRenderDrawColor(renderer, 219, 84, 97, 0);

  gridmax = GRIDMAX / 3;
#if 0
    boxmax_y = SUDYMAX * 3;
    boxmax_x = SUDXMAX * 3;
#else
  boxmax_y = SUDYMAX;
  boxmax_x = SUDXMAX;
#endif
  for (int ybox = 0; ybox < boxmax_y; ++ybox) {
    int ybase = ybox * gridmax;
    grect.y = ybase + gridmax / 3;
    grect.h = gridmax / 3;

    for (int xbox = 0; xbox < boxmax_x; ++xbox) {
      int chr = brd[ybox][xbox];
      if (chr != 0)
        buf[0] = chr + '0';
      else
        buf[0] = ' ';
      buf[1] = 0;

      TTF_Font *font = TTF_OpenFont("Sans.ttf",24);
      get_text_and_rect(renderer, 0, 0, buf, font, &texture, &trect);

      int xbase = xbox * gridmax;
      grect.x = xbase + gridmax / 3;
      grect.w = gridmax / 3;

      SDL_RenderCopy(renderer, texture, &trect, &grect);

      SDL_DestroyTexture(texture);
    }
  }
}

// imgsave -- save image to P6 .ppm file
void imgsave(SDL_Renderer *renderer, int imgno) {
  int pitch;
  FILE *xf;
  char file[1000];
  pixel_t *pixmap;

  pitch = sizeof(pixel_t) * WINDOW_WIDTH;
  pixmap = malloc(pitch * WINDOW_HEIGHT);

  // get pixel map from renderer image
#ifdef PIX32
  SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, pixmap, pitch);
#else
  SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGB24, pixmap, pitch);
#endif

  sprintf(file, "img%3.3d.ppm", imgno);
  xf = fopen(file, "w");

  fprintf(xf, "P6\n");
  fprintf(xf, "%d %d", WINDOW_WIDTH, WINDOW_HEIGHT);
  fprintf(xf, " %d\n", 255);

#ifdef PIX32
  pixel_t *pixcur = &pixmap[0];
  pixel_t *pixlim = &pixmap[WINDOW_WIDTH * WINDOW_HEIGHT];
  for (; pixcur < pixlim; ++pixcur) {
    fputc(pixcur->r, xf);
    fputc(pixcur->g, xf);
    fputc(pixcur->b, xf);
  }
#else
  fwrite(pixmap, sizeof(pixel_t), WINDOW_WIDTH * WINDOW_HEIGHT, xf);
#endif

  fclose(xf);

  free(pixmap);
}


int main(int argc, char **argv) {
  SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window *window;
  int quit;

  if (argc != 2){
    fprintf(stderr, "use case : ./sudrend <filename>\n");
    exit(EXIT_FAILURE);
  }

  char* filename = argv[1];

  /* Inint TTF. */
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window,
                              &renderer);


#if 0
    SDL_Rect rect1, rect2;
    SDL_Texture *texture1, *texture2;
    get_text_and_rect(renderer, 0, 0, "hello", font, &texture1, &rect1);
    get_text_and_rect(renderer, 0, rect1.y + rect1.h, "world", font,
        &texture2, &rect2);
#endif

  quit = 0;
  while (1) {
    while (SDL_PollEvent(&event) == 1) {
      if (event.type == SDL_QUIT)
        quit = 1;
    }
    if (quit)
      break;

    // set background
    SDL_SetRenderDrawColor(renderer, 247, 235, 232, 0);
    SDL_RenderClear(renderer);

    drawbox(renderer);
    drawgrid(renderer);
    drawtext(renderer);

    /* Use TTF textures. */
#if 0
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);
#endif

    imgsave(renderer, 0);

    SDL_RenderPresent(renderer);
  }

  /* Deinit TTF. */
#if 0
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
#endif
  TTF_Quit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
