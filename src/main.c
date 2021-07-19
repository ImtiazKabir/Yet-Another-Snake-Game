#define SDL_MAIN_HANDLED

#include "core.h"

#ifdef USING_EMSCRIPTEN
#include <emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef USING_EMSCRIPTEN
bool __quit_window__ = false;
#endif
int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

  SDL_Window *window = SDL_CreateWindow(TITLE_OF_WINDOW, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) __PRINT_ERROR__("Creating the window with SDL_CreateWindow");
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) __PRINT_ERROR__("Creating the renderer with SDL_CreateRenderer");

#ifdef USING_EMSCRIPTEN
  emscripten_set_main_loop_arg(main_loop, renderer, 0, 1);
#else
  while (!__quit_window__) {
    main_loop(renderer);
  }
#endif

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
