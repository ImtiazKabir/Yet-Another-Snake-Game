#include <stdio.h>

#include "core.h"
#include "stb/stbttf.h"
#include "stc/clist.h"

void clean_up(clist_pl* snake, STBTTF_Font* font) {
  c_foreach(it, clist_pl, *snake) {
    Pos_destroy(*it.ref);
  }
  clist_pl_del(snake);

  STBTTF_CloseFont(font);
}
