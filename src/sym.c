#include "includes.h"

dlist_t *names = NULL;
dlist_t *glob = NULL;

int addname(char *name)
{
    if (!names)
    {
        names = malloc(sizeof(dlist_t));
        dl_init(names, sizeof(char *));
    }

    for (int i = 0; i < names->used; i++)
    {
        if (!strncmp(*(char **)names->get[i], name, strlen(name)))
        {
            return i;
        }
    }

    char *final = malloc((strlen(name) + 2) * sizeof(char));

    strcpy(final, name);
    if (strcmp(name, "main"))
    {
        final[strlen(name) * sizeof(char)] = '_';
        final[(strlen(name) + 1) * sizeof(char)] = 0;
    }

    dl_add(names, &final);
    return names->used - 1;
}

int addlocl(int type, int class, char *name, int size)
{
    sym_t sym;
    sym.type = type;
    sym.class = class;
    sym.name = name;
    sym.size = size;

    sym.offs = 0;
    for (int i = 0; i < func->local->used; i++)
    {
        sym_t *local = func->local->get[i];
        sym.offs += type2size(local->type) * size;
    }

    dl_add(func->local, &sym);
}

int findlocl(char *name)
{
    for (int i = 0; i < func->local->used; i++)
    {
        sym_t *sym = func->local->get[i];
        if (!strncmp(name, sym->name, strlen(name)))
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

    if (class == C_EXTN)
    {
        name[strlen(name) - 1] = 0;
    }

    sym_t sym;
    sym.type = type;
    sym.class = class;
    sym.name = name;
    sym.argc = argc;
    sym.local = local;
    sym.root = root;
    sym.size = size;

    dl_add(glob, &sym);
    return glob->used - 1;
}
