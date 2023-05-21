#include "includes.h"

int level;

Node *if_stmt(void)
{
    Tok *t;
    Node *left, *mid, *right = NULL;

    next(&t);        // if
    next(&t);        // (
    mid = binexp(0); // condition
    next(&t);        // )
    left = block_stmt();

    if (tokseq(1, LT_ELSE))
    {
        next(&t); // else
        right = block_stmt();
    }

    return mknode(ST_IF, left, mid, right);
}

Node *while_stmt(void)
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

Node *for_stmt(void)
{
    Tok *t;
    Node *left, *mid, *right;

    next(&t); // for
    next(&t); // (

    next(&t); // variable decl (+ assignment) or just assignment
    back();
    if (istype(t->token))
    {
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

Node *ret_stmt(void)
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

Node *label_stmt(void)
{
    Tok *ident, *t;

    next(&ident); // identifier
    next(&t);     // colon

    ident->token = ST_IDENT;
    addlocl((Type){NULL}, K_LABL, C_LOCL, *(char **)uniq->get[ident->val.i], NULL);
    return mknode(ST_LABEL, mkleaf(ident, 0), NULL, NULL);
}

Node *goto_stmt(void)
{
    Tok *ident, *t;

    next(&t);     // goto
    next(&ident); // identifier
    next(&t);     // semicolon

    ident->token = ST_IDENT;
    return mknode(ST_GOTO, mkleaf(ident, 0), NULL, NULL);
}

Node *block_stmt(void)
{
    Tok *t;
    Sym *sym;
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
        case LT_GOTO:
            right = goto_stmt();
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
        case LT_BREAK:
            right = mknode(ST_BREAK, NULL, NULL, NULL);
            next(&t);
            next(&t);
            break;
        case LT_CONTINUE:
            right = mknode(ST_CONTINUE, NULL, NULL, NULL);
            next(&t);
            next(&t);
            break;
        case LT_ASM:
            next(&t);
            t->token = ST_ASM;
            right = mkleaf(t, NULL);
            break;
        case LT_RBRACE:
            next(&t);
            return root;
        case LT_IDENT:
            sym = getsym(t);

            if (sym->kind == K_LABL)
            {
                right = label_stmt();
                break;
            }

            if (sym->kind == K_TDEF)
            {
                t->token = sym->type.type;
                continue;
            }
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
