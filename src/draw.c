#include <SDL2/SDL.h>
#include <stdio.h>

#include "core.h"
#include "pos.h"
#include "stb/stbttf.h"
#include "stc/clist.h"

void draw(
    SDL_Renderer *renderer,
    const Pos *food,
    SDL_Texture *food_img,
    const clist_pl *snake,
    STBTTF_Font *font,
    const int score)

{

  // all the variables
  static SDL_Rect cell = {.w = WIDTH / col, .h = HEIGHT / row};

  if (SDL_SetRenderDrawColor(renderer, 137, 166, 105, 255) == -1)
    __PRINT_ERROR__("Setting the background color with SDL_SetRenderDrawColor");
  if (SDL_RenderClear(renderer) == -1)
    __PRINT_ERROR__("Clearing the window for background with SDL_RenderClear");

  // draw the food
  cell.x = food->j * cell.w;
  cell.y = food->i * cell.h;
  if (SDL_RenderCopy(renderer, food_img, NULL, &cell) == -1)
    __PRINT_ERROR__("drawing the food with SDL_RenderCopy");

  // draw the snake
  if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) == -1)
    __PRINT_ERROR__("Setting the snake body color with SDL_SetRenderDrawColor");

  c_foreach(it, clist_pl, *snake) {
    Pos *p = *it.ref;
    cell.x = p->j * cell.w;
    cell.y = p->i * cell.h;
    if (SDL_RenderFillRect(renderer, &cell) == -1)
      __PRINT_ERROR__("Drawing the snake body with SDL_RenderFillRect");
  }

  // draw the head
  Pos *head = *clist_pl_begin(snake).ref;
  cell.x = head->j * cell.w;
  cell.y = head->i * cell.h;
  if (SDL_SetRenderDrawColor(renderer, 46, 45, 39, 255) == -1)
    __PRINT_ERROR__("Setting the snake head color with SDL_SetRenderDrawColor");
  if (SDL_RenderFillRect(renderer, &cell) == -1)
    __PRINT_ERROR__("Drawing the snake head with SDL_RenderFillRect");

  // show the score
  int length = snprintf(NULL, 0, "%d", score);
  char str[length + 1];
  sprintf(str, "%d", score);
  if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) == -1)
    __PRINT_ERROR__("Setting the text color with SDL_SetRenderDrawColor");
  STBTTF_RenderText(renderer, font, 20, 40, str);
}
