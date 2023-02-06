#ifndef __SYM_
#define __SYM_

extern dlist_t *glob;

typedef struct
{
    int type;
    char *name;
} sym_t;

int findglob(char *name);
void addglob(int type, char *name);

#endif
