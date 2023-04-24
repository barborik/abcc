#include "includes.h"

dlist_t *uniq = NULL;
dlist_t *glob = NULL;

int adduniq(char *str)
{
    if (!uniq)
    {
        uniq = malloc(sizeof(dlist_t));
        dl_init(uniq, sizeof(char *));
    }

    for (int i = 0; i < uniq->used; i++)
    {
        if (!strcmp(*(char **)uniq->get[i], str))
        {
            return i;
        }
    }

    char *final = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(final, str);

    dl_add(uniq, &final);
    return uniq->used - 1;
}

Sym *addlocl(int type, int addr, int complex, int class, char *name, int size)
{
    Sym sym;
    sym.type.type = type;
    sym.type.addr = addr;
    sym.class = class;
    sym.name = name;
    sym.size = size;
    sym.level = level;
    sym.offs = 0;
    sym.type.complex = complex;

    dl_add(func->local, &sym);
    return func->local->get[func->local->used - 1];
}

Sym *findlocl(char *name)
{
    for (int i = 0; i < func->local->used; i++)
    {
        Sym *sym = func->local->get[i];
        if (!strcmp(name, sym->name))
        {
            return sym;
        }
    }

    return NULL;
}

// returns index of the global symbol, -1 if not found
Sym *findglob(char *name)
{
    if (!glob)
    {
        glob = malloc(sizeof(dlist_t));
        dl_init(glob, sizeof(Sym));
    }

    for (int i = 0; i < glob->used; i++)
    {
        Sym *sym = glob->get[i];
        if (!strncmp(name, sym->name, strlen(name)))
        {
            return sym;
        }
    }

    return NULL;
}

// adds a global symbol to the table and returns its index
Sym *addglob(int type, int addr, int complex, int class, char *name, int size, int argc, Node *root, dlist_t *local)
{
    if (!glob)
    {
        glob = malloc(sizeof(dlist_t));
        dl_init(glob, sizeof(Sym));
    }

    dlist_t *stack = malloc(sizeof(dlist_t));
    dl_init(stack, sizeof(Sym *));

    Sym sym;
    sym.type.type = type;
    sym.type.addr = addr;
    sym.class = class;
    sym.name = name;
    sym.argc = argc;
    sym.local = local;
    sym.root = root;
    sym.size = size;
    sym.stack = stack;
    sym.level = 1;
    sym.type.complex = complex;

    dl_add(glob, &sym);
    return glob->get[glob->used - 1];
}

Sym *getsym(Tok *t)
{
    Sym *sym = NULL;
    char **name = *(char **)uniq->get[t->val.i];

    sym = findlocl(name);
    if (!sym) sym = findglob(name);
    return sym;
}

Sym *getsymc(Tok *t, int class)
{
    char **name = *(char **)uniq->get[t->val.i];

    switch (class)
    {
    case C_LOCL:
        return findlocl(name);
    case C_GLOB:
    case C_EXTN:
    case C_DATA:
        return findglob(name);
    }

    return NULL;
}

int globid(Tok *t)
{
    Sym *sym;
    char **name = *(char **)uniq->get[t->val.i];

    for (int i = 0; i < glob->used; i++)
    {
        sym = glob->get[i];
        if (sym->name == name)
        {
            return i;
        }
    }

    return -1;
}
