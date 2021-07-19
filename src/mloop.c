#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTTF_IMPLEMENTATION

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef USING_EMSCRIPTEN
#include <emscripten.h>
#endif

#include "core.h"
#include "pos.h"
#include "stb/stbttf.h"
#include "stc/clist.h"

void main_loop(const void *v_renderer) {
  SDL_Renderer *renderer = (SDL_Renderer *)v_renderer;

  // all the variables
  static Pos food;
  static SDL_Texture *food_img;
  static clist_pl snake;
  static DIR vel = RIGHT;
  static bool game_over = false;
  static int score = 0;
  static STBTTF_Font *font;

  // setup portion
  static bool __run_setup__ = true;
  if (__run_setup__) {
    setup(renderer, &food, &food_img, &snake, &font);
    __run_setup__ = false;
  }

  // clean up portion
  if (event_handler(&vel)) {
    clean_up(&snake, font);
#ifdef USING_EMSCRIPTEN
    emscripten_cancel_main_loop();
#else
    __quit_window__ = true;
    return;
#endif
  }

  // other portion
  if (!game_over) {
    update(&snake, vel, &food, &game_over, &score);
    draw(renderer, &food, food_img, &snake, font, score);
    SDL_Delay(1000 / FPS);
    SDL_RenderPresent(renderer);
  }
}
