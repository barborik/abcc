/*
+---------------------------------------------+
|                                             |
| Adam Barborík's dynamic list implementation |
|                                             |
+---------------------------------------------+
*/

#ifndef __DLIST_
#define __DLIST_

#include <stdlib.h>
#include <string.h>

typedef struct // dynamic list structure
{
    void **get;  // data buffer
    int isize;   // item size in bytes
    size_t used; // how many items are there
    size_t size; // actual size of the data buffer in items (not bytes)
} dlist_t;

// [OUT] dlist_t *list - list you want to initialize
// [IN]  int isize - item size
static void dl_init(dlist_t *list, int isize)
{
    list->isize = isize;
    list->used = 0;
    list->size = 100;
    list->get = malloc(sizeof(void *) * list->size);
}

// [IN] dlist_t *list - list you want to grow
// [IN] size_t num - number of items to grow by (can be negative to shrink)
static void dl_grow(dlist_t *list, size_t num)
{
    list->size += num;
    list->get = realloc(list->get, sizeof(void *) * list->size);
}

// [IN] dlist_t *list - list to which you want to add
// [IN] void *item - item to add
static void dl_add(dlist_t *list, void *item)
{
    if (list->used == list->size)
    {
        dl_grow(list, 100);
    }

    list->get[list->used] = malloc(list->isize);
    memcpy(list->get[list->used], item, list->isize);
    list->used++;
}

// [IN] dlist_t *list - list from which you want to remove
// [IN] size_t index - index of the item
static void dl_rem(dlist_t *list, size_t index)
{
    free(list->get[index]);
    for (size_t i = 0; i < list->used - index; i++)
    {
        list->get[index + i] = list->get[index + i + 1];
    }
    list->used--;

    if (list->size > list->used + 100)
    {
        dl_grow(list, -100);
    }
}

// [IN] dlist_t *list - list where elements will be swapped
// [IN] size_t index1 - index of first item
// [IN] size_t index2 - index of second item
static void dl_swp(dlist_t *list, size_t index1, size_t index2)
{
    void *tmp = list->get[index1];
    list->get[index1] = list->get[index2];
    list->get[index2] = tmp;
}

// [IN] dlist_t *list - list to be inserted into
// [IN] void *item - item to be inserted
// [IN] size_t index - index where the item should be inserted
static void dl_ins(dlist_t *list, void *item, size_t index)
{
    if (list->used == list->size)
    {
        dl_grow(list, 10);
    }
    list->used++;

    for (size_t i = list->used - 1; i > index; i--)
    {
        dl_swp(list, i, i - 1);
    }

    list->get[index] = malloc(list->isize);
    memcpy(list->get[index], item, list->isize);
}

// [IN] dlist_t *list - list to be cleared
static void dl_clear(dlist_t *list)
{
    for (int i = list->used; i > 0; i--)
    {
        dl_rem(list, i);
    }
}

// [IN] dlist_t *list - list to free
static void dl_free(dlist_t *list)
{
    for (size_t i = 0; i < list->used; i++)
    {
        free(list->get[i]);
    }
    free(list->get);
}

#endif