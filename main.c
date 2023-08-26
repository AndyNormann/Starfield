#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <stdbool.h>

//
// Window variables
//
SDL_Window* window;
SDL_Surface * window_surface;

unsigned int* pixels;
int windowHeight;
int windowWidth;

//
// Color data
//
typedef struct color_ {
  unsigned int R;
  unsigned int G;
  unsigned int B;
  unsigned int alpha;
} color;

color WHITE;
color BLACK;
color RED;

void initColors() {
  WHITE.R = 255;
  WHITE.G = 255;
  WHITE.B = 255;
  WHITE.alpha = 255;

  BLACK.R = 0;
  BLACK.G = 0;
  BLACK.B = 0;
  BLACK.alpha = 0;

  RED.R = 255;
  RED.G = 0;
  RED.B = 0;
  RED.alpha = 0;
}

//
// Drawing utilities
//
static inline void setPixel(int x, int y, color c) {
  pixels[x + y * windowWidth] =
    SDL_MapRGBA(window_surface->format, c.R, c.G, c.B, c.alpha);
}

void drawRectangle(int x, int y, int height, int width, color c) {
  for(int i = x; i < x + width; i++) {
    for(int j = y; j < y + height; j++) {
      setPixel(i, j, c);
    }
  }
}

void drawStar(int screenX, int screenY, int size, color c) {

  setPixel(screenX, screenY, c);
  if(size >= 1) {
    setPixel(screenX + 1, screenY, c);
    setPixel(screenX - 1, screenY, c);
  } 
  if(size >= 2) {
    setPixel(screenX, screenY + 1, c);
    setPixel(screenX, screenY - 1, c);
  } 
  if(size >= 3) {
    setPixel(screenX + 1, screenY + 1, c);
    setPixel(screenX + 1, screenY - 1, c);
  }
  if(size >= 4) {
    setPixel(screenX - 1, screenY + 1, c);
    setPixel(screenX - 1, screenY - 1, c);
  }
  if(size >= 5) {
    setPixel(screenX + 2, screenY, c);
    setPixel(screenX - 2, screenY, c);
  }
  if(size >= 6) {
    setPixel(screenX, screenY + 2, c);
    setPixel(screenX, screenY - 2, c);
  }
  if(size >= 7) {
    setPixel(screenX + 2, screenY + 2, c);
    setPixel(screenX + 2, screenY - 2, c);
  }
  if(size >= 8) {
    setPixel(screenX - 2, screenY + 2, c);
    setPixel(screenX - 2, screenY - 2, c);
  }
}

//
// Stars
//

#define STAR_AMOUNT 800

typedef struct star_ {
  float x;
  float y;
  float z;
  color c;
} star;

star* stars;

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  initColors();

  stars = malloc(sizeof(star) * STAR_AMOUNT);

  window = SDL_CreateWindow("Test",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      1280, 720,
      SDL_WINDOW_RESIZABLE);

  window_surface = SDL_GetWindowSurface(window);

  pixels = window_surface->pixels;
  windowWidth = window_surface->w;
  windowHeight = window_surface->h;

  srand(time(NULL));

  // Init stars
  for(int i = 0; i < STAR_AMOUNT; i++) {
    stars[i].x = (float)(rand() % 100) / 100;
    stars[i].y = (float)(rand() % 100) / 100;
    stars[i].z = ((float)(rand() % 20) / 100) + 0.2;
    if(i % 10 == 0) {
      stars[i].c = RED;
    } else {
      stars[i].c = WHITE;
    }
  }

  /* printf("Pixel format: %s\n", SDL_GetPixelFormatName(window_surface->format->format)); */

  clock_t before = clock();

  while (1)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT) exit(0);
      if (event.type == SDL_WINDOWEVENT)
      {
        if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
          window_surface = SDL_GetWindowSurface(window);
          pixels = window_surface->pixels;
          windowWidth = window_surface->w;
          windowHeight = window_surface->h;
        }
      }
      if(event.type == SDL_KEYDOWN) {
        int sym = event.key.keysym.sym;
        switch(sym) {
          case SDLK_ESCAPE:
          case 113:
            exit(0);
        }
      }
    }

    // Set every pixel to black.
    for (int y = 0; y < windowHeight; ++y) {
      for (int x = 0; x < windowWidth; ++x) {
        setPixel(x, y, BLACK);
      }
    }

    clock_t difference = (clock() - before) * 1000 / CLOCKS_PER_SEC;

    // Draw stars
    for(int i = 0; i < STAR_AMOUNT; i++) {
      if(difference > 50) {
        stars[i].z -= 0.003;
        before = clock();
      }

      int screenX = (stars[i].x * windowWidth) / stars[i].z;
      int screenY = (stars[i].y * windowHeight) / stars[i].z;

      if(screenX >= 1 && screenX < windowWidth && screenY >= 1 && screenY < windowHeight-1) {
        drawStar(screenX, screenY, (int)(stars[i].z * 10) % 8, stars[i].c);
      } else {
        stars[i].z = ((float)(rand() % 20) / 100) + 0.8;
      }
    }

    SDL_UpdateWindowSurface(window);
  }
}
