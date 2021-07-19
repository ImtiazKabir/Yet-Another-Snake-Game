#include "pos.h"

#include <stdlib.h>

Pos *Pos_create(int i, int j) {
  Pos *p = (Pos *)malloc(sizeof *p);
  p->i = i;
  p->j = j;
  return p;
}

void Pos_destroy(Pos *self) {
  free(self);
}
