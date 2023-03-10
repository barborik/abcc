#ifndef __MAP_
#define __MAP_

int un2stx(token_t *t);
int bin2stx(token_t *t);
int stmt2stx(token_t *t);

int val2ptr(token_t *t);
int ptr2val(token_t *t);

#endif