#ifndef __SYM_
#define __SYM_

extern dlist_t *names;
extern dlist_t *glob;

enum
{
    C_VAR,
    C_FUNC,
};

typedef struct
{
    int type;   // data type
    int class;  // C_VAR or C_FUNC
    char *name; // symbol name

    /* FUNCTION ONLY */
    dlist_t *args;  // function arguments
    dlist_t *local; // local symbols of a function
    asnode_t *root; // root of the function ast
} sym_t;

int addname(char *name);
int addglob(int type, int class, char *name, dlist_t *args, dlist_t *local, asnode_t *root);
int findglob(char *name);

#endif
