#include "includes.h"

int level;

Node *if_stmt()
{
    Tok *t;
    Node *left, *mid, *right;

    next(&t);        // if
    next(&t);        // (
    mid = binexp(0); // condition
    next(&t);        // )
    left = block_stmt();

    return mknode(ST_IF, left, mid, NULL);
}

Node *while_stmt()
{
    Tok *t;
    Node *left, *mid, *right;

    next(&t);        // while
    next(&t);        // (
    mid = binexp(0); // condition
    next(&t);        // )
    left = block_stmt();

    return mknode(ST_WHILE, left, mid, NULL);
}

Node *ret_stmt()
{
    Tok *tok;
    Node *left = NULL;

    next(&tok); // return
    if (!tokseq(1, LT_SEMICOLON))
    {
        left = binexp(0); // expression
        next(&tok);       // ;
    }
    else
    {
        next(&tok); 
    }

    return mknode(ST_RETURN, left, NULL, NULL);
}

Node *block_stmt()
{
    Tok *t;
    Node *right, *root = NULL;

    level++;
    next(&t);

    while (next(&t))
    {
        back();

        switch (t->token)
        {
        case LT_I0:
        case LT_I8:
        case LT_I16:
        case LT_I32:
        case LT_I64:
        case LT_U0:
        case LT_U8:
        case LT_U16:
        case LT_U32:
        case LT_U64:
            right = var_decl(C_LOCL);
            break;
        case LT_IF:
            right = if_stmt();
            break;
        case LT_WHILE:
            right = while_stmt();
            break;
        case LT_RETURN:
            right = ret_stmt();
            break;
        case LT_RBRACE:
            next(&t);
            return root;
        default:
            right = binexp(0);
            next(&t);
        }

        if (!root)
        {
            root = right;
        }
        else
        {
            root = mknode(ST_JOIN, root, NULL, right);
        }
    }
}
