#ifndef __SYM_
#define __SYM_

extern dlist_t *uniq;
extern dlist_t *glob;

enum
{
    C_GLOB, // global symbol
    C_LOCL, // local symbol
    C_FUNC, // function (or procedure) symbol
    C_EXTN, // extern symbol
    C_DATA, // data section symbol (char and string literals)
};

typedef struct
{
    int type;   // data type
    int size;   // number of elements
    int class;  // one of the values from the enum above
    char *name; // symbol name

    /* OTHER */
    int array; // is array boolean

    /* SHARED */
    int level; // indentation level of the symbol

    /* LOCAL ONLY */
    int offs; // stack offset of a local symbol

    /* FUNCTION ONLY */
    int argc;       // number of arguments
    dlist_t *local; // local symbols of a function including arguments
    dlist_t *stack; // simulates a function stack, adding and removing elements while generating
    asnode_t *root; // root of the function ast
} sym_t;

int adduniq(char *str);
int addglob(int type, int class, char *name, int size, int argc, dlist_t *local, asnode_t *root);
int addlocl(int type, int class, char *name, int size);
int findglob(char *name);
int findlocl(char *name);

#endif
