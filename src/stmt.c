#include "includes.h"

asnode_t *assign_stmt()
{
    token_t t;
    asnode_t *root, *left, *right;

    next(&t);
    left = mknode(&t, NULL, NULL);

    next(&t);
    right = binexp(0);
    root = mknode(&t, left, right);

    gen(root);
}