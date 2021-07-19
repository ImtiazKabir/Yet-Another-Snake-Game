#include <stdbool.h>
#include <stdio.h>

#include "core.h"
#include "pos.h"
#include "stc/clist.h"

void update(clist_pl *snake, const DIR vel, Pos *food, bool *game_over, int *score) {
  Pos *phead = *clist_pl_begin(snake).ref;
  int ni = phead->i, nj = phead->j;

  // check food
  bool eaten = false;
  if (ni == food->i && nj == food->j) {
    generate_food(food);
    eaten = true;
    ++(*score);
  }

  // move the snake
  switch (vel) {
    case UP:
      --ni;
      break;
    case LEFT:
      --nj;
      break;
    case DOWN:
      ++ni;
      break;
    case RIGHT:
      ++nj;
      break;
  }

  ni = (ni % row + row) % row;
  nj = (nj % col + col) % col;
  Pos *nhead = Pos_create(ni, nj);

  c_foreach(it, clist_pl, *snake) {
    Pos *p = *it.ref;
    if (p->i == nhead->i && p->j == nhead->j) *game_over = true;
  }

  clist_pl_push_front(snake, nhead);

  // get rid of the tail
  if (!eaten) {
    Pos *tail = *clist_pl_back(snake);
    clist_pl_remove(snake, tail);
    Pos_destroy(tail);
  }
}
