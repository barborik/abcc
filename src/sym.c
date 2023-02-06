#include "includes.h"

dlist_t *glob = NULL;

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
        if (!strcmp(name, ((sym_t *)glob->get[i])->name))
        {
            return i;
        }
    }

    return -1;
}

// adds a global symbol to the table
void addglob(int type, char *name)
{
    if (!glob)
    {
        glob = malloc(sizeof(dlist_t));
        dl_init(glob, sizeof(sym_t));
    }

    char *final = malloc(strlen(name) * sizeof(char) + 1);
    strcpy(final, name);

    sym_t sym;
    sym.type = type;
    sym.name = final;

    dl_add(glob, &sym);
}
