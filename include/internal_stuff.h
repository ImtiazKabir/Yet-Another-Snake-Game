#ifndef F59FC23E_9CE8_4D68_B629_99183D3EC9C8
#define F59FC23E_9CE8_4D68_B629_99183D3EC9C8

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

//* Internal variables of the sketch, should not be changed

#ifndef USING_EMSCRIPTEN
extern bool __quit_window__;
#endif
//* ------------------------------------------------------

static inline void __PRINT_ERROR__(const char *func_name) {
  fprintf(stderr, "%s did not work\n---------------\n%s\n\n\n\n", func_name, SDL_GetError());
  SDL_ClearError();
}

static inline void __CONSOLE_PRINT__(const char *msg) {
  fprintf(stderr, "--> %s\n\n\n\n", msg);
}

static inline void __PROGRAM_REACHED__() {
  fprintf(stderr, "__PROGRAM_REACHED__ successful\n\n\n\n");
}

char *__get_path__(const char *child);

#endif /* F59FC23E_9CE8_4D68_B629_99183D3EC9C8 */
