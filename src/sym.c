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
        if (!strncmp(names->get[i], name, strlen(name)))
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
int addglob(int type, int class, char *name, dlist_t *args, dlist_t *local, asnode_t *root)
{
    if (!glob)
    {
        glob = malloc(sizeof(dlist_t));
        dl_init(glob, sizeof(sym_t));
    }

    sym_t sym;
    sym.type = type;
    sym.class = class;
    sym.name = name;
    sym.args = args;
    sym.local = local;
    sym.root = root;

    dl_add(glob, &sym);
    return glob->used - 1;
}
