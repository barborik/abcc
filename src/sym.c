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
        sym_t *sym = glob->get[i];
        if (!strncmp(name, sym->name, strlen(name)))
        {
            return i;
        }
    }

    return -1;
}

// adds a global symbol to the table and returns the assigned id
void addglob(char *name)
{
    if (!glob)
    {
        glob = malloc(sizeof(dlist_t));
        dl_init(glob, sizeof(sym_t));
    }

    char *final = malloc((strlen(name) + 2) * sizeof(char));
    strcpy(final, name);

    if (strcmp(name, "main"))
    {
        final[strlen(name) * sizeof(char)] = '_';
        final[(strlen(name) + 1) * sizeof(char)] = 0;
    }

    sym_t sym;
    sym.func = 0;
    sym.name = final;

    dl_add(glob, &sym);
}
