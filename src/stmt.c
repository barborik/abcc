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

Node *for_stmt()
{
    Tok *t;
    Node *left, *mid, *right;

    next(&t); // for
    next(&t); // (

    next(&t); // variable decl (+ assignment) or just assignment
    if (istype(t->token))
    {
        back();
        left = var_decl(C_LOCL, (Type){NULL});
    }
    else
    {
        left = binexp(0);
        next(&t); // ;
    }

    mid = binexp(0);   // condition
    next(&t);          // ;
    right = binexp(0); // increment/decrement
    next(&t);          // )

    left = mknode(ST_JOIN, left, NULL, block_stmt());
    return mknode(ST_FOR, left, mid, right);
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
            right = var_decl(C_LOCL, (Type){NULL});
            break;
        case LT_IF:
            right = if_stmt();
            break;
        case LT_WHILE:
            right = while_stmt();
            break;
        case LT_FOR:
            right = for_stmt();
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
            break;
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
