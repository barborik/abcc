#include "includes.h"

dlist_t *glob = NULL;

// returns index of the global symbol, -1 if not found
int findglob(char *name)
{
    if (!glob)
    {
        glob = malloc(sizeof(dlist_t));
        dl_init(glob, sizeof(char *));
    }

    for (int i = 0; i < glob->used; i++)
    {
        if (!strcmp(name, glob->get[i]))
        {
            return i;
        }
    }

    return -1;
}

// adds a global symbol to the table
void addglob(char *name)
{
    if (!glob)
    {
        glob = malloc(sizeof(dlist_t));
        dl_init(glob, sizeof(char *));
    }

    char *final = malloc(strlen(name) * sizeof(char) + 1);
    strcpy(final, name);
    dl_add(glob, final);
}