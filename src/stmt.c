#include "includes.h"

asnode_t *assign_stmt()
{
    token_t *t;
    asnode_t *root, *left, *right;

    // ident
    next(&t);
    t->token = ST_LVIDENT;
    right = mknode(t, NULL, NULL, NULL);

    // equals sign + right side of the statement
    next(&t);
    left = binexp(0);

    root = mknode(t, left, NULL, right);

    // gen(root, -1);
    // rfree_all();

    // semicolon
    next(&t);
    // handle error if (t.token != T_SEMICOLON) ERROR

    return root;
}

asnode_t *cond_stmt()
{
    token_t *t;
    asnode_t *root, *left, *mid, *right;

    next(&t); // if or while
    stmt2stx(t);
    root = mknode(t, NULL, NULL, NULL);
    next(&t);        // (
    mid = binexp(0); // condition
    next(&t);        // )
    left = block_stmt();

    root->mid = mid;
    root->left = left;
    return root;
}

/*asnode_t *stmt()
{
}*/

asnode_t *block_stmt()
{
    token_t *t;
    asnode_t *right, *root = NULL;

    token_t *join = malloc(sizeof(token_t));
    join->token = ST_JOIN;

    next(&t);

    while (next(&t))
    {
        back();
        right = NULL;

        switch (t->token)
        {
        case T_I0:
        case T_I8:
        case T_I16:
        case T_I32:
        case T_I64:
        case T_U0:
        case T_U8:
        case T_U16:
        case T_U32:
        case T_U64:
            var_decl();
            break;
        case T_IDENT:
            right = assign_stmt();
            break;
        case T_IF:
            right = cond_stmt();
            break;
        case T_WHILE:
            right = cond_stmt();
            break;
        case T_RBRACE:
            next(&t);
            return root;
        }

        if (right)
        {
            if (!root)
            {
                root = right;
            }
            else
            {
                root = mknode(join, root, NULL, right);
            }
        }
    }
}
