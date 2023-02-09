#include "includes.h"

void assign_stmt()
{
    token_t *t;
    asnode_t *root, *left, *right;

    // ident
    next(&t);
    t->token = T_LVIDENT;
    right = mknode(t, NULL, NULL);

    // equals sign + right side of the statement
    next(&t);
    left = binexp(0);
    root = mknode(t, left, right);

    gen(root, -1);
    rfree_all();

    // semicolon
    next(&t);
    // handle error if (t.token != T_SEMICOLON) ERROR
}
