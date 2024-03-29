#ifndef __SYM_
#define __SYM_

extern dlist_t *uniq;
extern dlist_t *glob;

enum
{
    C_GLOB, // global symbol
    C_LOCL, // local symbol
    C_EXTN, // extern symbol
    C_DATA, // data section symbol (char and string literals)
};

enum
{
    K_VAR,
    K_FUNC,
    K_LABL,
    K_TDEF,
    K_ENUM,
};

struct type
{
    int type;
    int addr;
    int size;
};

struct sym
{
    Type type;  // type of symbol
    int kind;   // one of the values from the enum above K_{...}
    int class;  // one of the values from the enum above C_{...}
    char *name; // symbol name

    /* SHARED */
    int level; // indentation level of the symbol or current indent level of function

    /* LOCAL ONLY */
    int offs; // stack offset of a local symbol

    /* FUNCTION ONLY */
    int argc;       // number of arguments
    Node *root;     // root of the function ast
    dlist_t *local; // local symbols of a function including arguments
    dlist_t *stack; // simulates a function stack, adding and removing elements while generating

    /* ENUM */
    int eval;
};

int adduniq(char *str);

Sym *addglob(Type type, int kind, int class, char *name, int argc, Node *root, dlist_t *local);
Sym *addlocl(Type type, int kind, int class, char *name);

Sym *findglob(char *name);
Sym *findlocl(char *name);

Sym *getsym(Tok *t);
Sym *getsymc(Tok *t, int class);

int globid(Tok *t);

#endif
