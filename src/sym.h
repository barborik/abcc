#ifndef __SYM_
#define __SYM_

extern dlist_t *glob;

typedef struct
{
    int type;       // data type
    int func;       // bool -> is function
    asnode_t *root; // root of the function ast
    char *name;
} sym_t;

int findglob(char *name);
void addglob(char *name);

#endif
