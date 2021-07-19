#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "pos.h"
#include "stb/stbttf.h"
#include "stc/clist.h"

void setup(SDL_Renderer *renderer, Pos *food, SDL_Texture **food_img, clist_pl *snake, STBTTF_Font **font) {
  // inititate randomness and generate the food
  srand((unsigned int)time(NULL));
  generate_food(food);


  // create the food_img
  char *path_to_food = __get_path__("assets/food.png");
  SDL_Surface *surface = IMG_Load(path_to_food);
  if (!surface) __PRINT_ERROR__("Creating the surface with IMG_Load");
  *food_img = SDL_CreateTextureFromSurface(renderer, surface);
  if (!*food_img) __PRINT_ERROR__("creating food texture with SDL_CreateTextureFromSurface");
  SDL_FreeSurface(surface);
  free(path_to_food);

  // create the snake
  *snake = clist_pl_init();
  Pos *head = Pos_create(col / 5, row / 2);
  clist_pl_push_back(snake, head);
  head = Pos_create(head->i, head->j - 1);
  clist_pl_push_back(snake, head);

  // create the font
  char *path_to_font = __get_path__("assets/DSEG7ClassicMini-Regular.ttf");
  *font = STBTTF_OpenFont(renderer, path_to_font, 32);
  if (!*font) __PRINT_ERROR__("opening font with STBTTF_OpenFont");
  free(path_to_font);
}
