#include <stdlib.h>

#include "core.h"
#include "pos.h"

void generate_food(Pos *food) {
  food->i = rand() % row;
  food->j = rand() % col;
}
