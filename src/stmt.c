#include "includes.h"

asnode_t *stmt()
{
    token_t *t;
    asnode_t *right, *root = NULL;

    token_t *glue = malloc(sizeof(token_t));
    glue->token = T_GLUE;

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
        }

        if (right)
        {
            if (!root)
            {
                root = right;
            }
            else
            {
                root = mknode(glue, root, right);
            }
        }
    }

    return root;
}

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
