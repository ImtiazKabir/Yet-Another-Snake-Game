#include <SDL2/SDL.h>

#include "core.h"
#include "stc/clist.h"

int event_handler(DIR *vel) {
  // return value 1 means terminate program
  SDL_Event event;
  DIR proxy = *vel;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return 1;
    } else if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          if (*vel != RIGHT) proxy = LEFT;
          break;
        case SDLK_RIGHT:
          if (*vel != LEFT) proxy = RIGHT;
          break;
        case SDLK_UP:
          if (*vel != DOWN) proxy = UP;
          break;
        case SDLK_DOWN:
          if (*vel != UP) proxy = DOWN;
          break;
        default:
          break;
      }
    }
  }
  *vel = proxy;
  return 0;
}
