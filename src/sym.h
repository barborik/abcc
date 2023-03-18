#ifndef __SYM_
#define __SYM_

extern dlist_t *names;
extern dlist_t *glob;

enum
{
    C_GLOB, // global symbol
    C_LOCL, // local symbol
    C_FUNC, // function (or procedure) symbol
    C_EXTN, // extern symbol
};

typedef struct
{
    int type;   // data type
    int class;  // one of the values from the enum above
    char *name; // symbol name

    /* LOCAL ONLY */
    int offs; // stack offset of a local symbol

    /* FUNCTION ONLY */
    int argc;       // number of arguments
    dlist_t *local; // local symbols of a function including arguments
    asnode_t *root; // root of the function ast
} sym_t;

int addname(char *name);
int addglob(int type, int class, char *name, int argc, dlist_t *local, asnode_t *root);
int findglob(char *name);
int findlocl(sym_t *func, char *name);

#endif
