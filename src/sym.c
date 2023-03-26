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

int addlocl(int type, int class, char *name, int size)
{
    sym_t sym;
    sym.type = type;
    sym.class = class;
    sym.name = name;
    sym.size = size;
    sym.level = level;
    sym.offs = 0;
    sym.prim = 1;

    dl_add(func->local, &sym);
    return func->local->used - 1;
}

int findlocl(char *name)
{
    for (int i = 0; i < func->local->used; i++)
    {
        sym_t *sym = func->local->get[i];
        if (!strcmp(name, sym->name))
        {
            return i;
        }
    }

    return -1;
}

// returns index of the global symbol, -1 if not found
int findglob(char *name)
{
    if (!glob)
    {
        glob = malloc(sizeof(dlist_t));
        dl_init(glob, sizeof(sym_t));
    }

    for (int i = 0; i < glob->used; i++)
    {
        sym_t *sym = glob->get[i];
        if (!strncmp(name, sym->name, strlen(name)))
        {
            return i;
        }
    }

    return -1;
}

// adds a global symbol to the table and returns its index
int addglob(int type, int class, char *name, int size, int argc, dlist_t *local, asnode_t *root)
{
    if (!glob)
    {
        glob = malloc(sizeof(dlist_t));
        dl_init(glob, sizeof(sym_t));
    }

    char *final = malloc((strlen(name) + 2) * sizeof(char));
    strcpy(final, name);
    if (strcmp(name, "main") && class != C_EXTN && (type != T_STRLIT || type != T_CHARLIT))
    {
        final[strlen(name) * sizeof(char)] = '_';
        final[(strlen(name) + 1) * sizeof(char)] = 0;
    }

    dlist_t *stack = malloc(sizeof(dlist_t));
    dl_init(stack, sizeof(sym_t *));

    sym_t sym;
    sym.type = type;
    sym.class = class;
    sym.name = final;
    sym.argc = argc;
    sym.local = local;
    sym.root = root;
    sym.size = size;
    sym.stack = stack;
    sym.level = 1;
    sym.prim = 1;

    dl_add(glob, &sym);
    return glob->used - 1;
}
