#ifndef D07D45CE_AA78_4D49_B29A_EBD0B6BCD4AD
#define D07D45CE_AA78_4D49_B29A_EBD0B6BCD4AD

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "internal_stuff.h"
#include "pos.h"
#include "stb/stbttf.h"
#include "stc/clist.h"

#define TITLE_OF_WINDOW "Classic Snake"
#define WIDTH (int)800
#define HEIGHT (int)400
#define FPS (uint32_t)10

#define col (int)20
#define row (int)10

using_clist(pl, Pos *);

typedef enum {UP, LEFT, DOWN, RIGHT} DIR;

void main_loop(const void *v_renderer);
int event_handler(DIR *vel);
void setup(SDL_Renderer *renderer, Pos *food, SDL_Texture **food_img, clist_pl *snake, STBTTF_Font **font);
void draw(SDL_Renderer *renderer, const Pos *food, SDL_Texture *food_img, const clist_pl *snake, STBTTF_Font *font, const int score);
void clean_up(clist_pl *snake, STBTTF_Font *font);

void generate_food(Pos *food);
void update(clist_pl *snake, const DIR vel, Pos *food, bool *game_over, int *score);

#endif /* D07D45CE_AA78_4D49_B29A_EBD0B6BCD4AD */
