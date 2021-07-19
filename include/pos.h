#ifndef C6CDD258_1CD5_4361_8EBE_7FDF50B82775
#define C6CDD258_1CD5_4361_8EBE_7FDF50B82775

typedef struct {
  int i;
  int j;
} Pos;

Pos *Pos_create(int i, int j);
void Pos_destroy(Pos *self);

#endif /* C6CDD258_1CD5_4361_8EBE_7FDF50B82775 */
