#ifndef __STM_
#define __STM_

extern int level;

// asnode_t *stmt();
void idsym(token_t *t);
asnode_t *block_stmt();
asnode_t *assign_stmt();
asnode_t *func_call(int semi);

#endif