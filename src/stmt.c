#include "includes.h"

asnode_t *assign_stmt()
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

    // gen(root, -1);
    // rfree_all();

    // semicolon
    next(&t);
    // handle error if (t.token != T_SEMICOLON) ERROR

    return root;
}

asnode_t *if_stmt()
{
    token_t *t;

    next(&t); // if
    next(&t); // (
    next(&t); // condition
    next(&t); // )
    next(&t); // {
    next(&t); // }
}

asnode_t *stmt()
{
    token_t *t;
    asnode_t *right, *root = NULL;

    token_t *join = malloc(sizeof(token_t));
    join->token = T_JOIN;

    while (next(&t))
    {
        back();
        right = NULL;

        switch (t->token)
        {
        case T_I64:
            var_decl();
            break;
        case T_IDENT:
            right = assign_stmt();
            break;
        case T_IF:
            right = if_stmt();
            break;
        }

        if (right)
        {
            if (!root)
            {
                root = right;
            }
            else
            {
                root = mknode(join, root, right);
            }
        }
    }

    return root;
}
