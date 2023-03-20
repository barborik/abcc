#include "includes.h"

void idsym(token_t *t)
{
    char **name = *(char **)names->get[t->val.id];

    t->class = C_LOCL;
    t->val.id = findlocl(name);

    if (t->val.id < 0)
    {
        t->class = C_GLOB;
        t->val.id = findglob(name);
    }
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
    left = block_stmt(func);

    root->mid = mid;
    root->left = left;
    return root;
}

asnode_t *explist()
{
    token_t *t;
    asnode_t *root = NULL, *right = NULL;

    token_t *join = malloc(sizeof(token_t));
    join->token = ST_JOIN;

    while (1)
    {
        if (tokseq(1, T_RPAR))
        {
            break;
        }

        right = binexp(0);
        root = mknode(join, root, NULL, right);

        if (tokseq(1, T_RPAR))
        {
            break;
        }
        next(&t);
    }

    return root;
}

asnode_t *func_call(int semi)
{
    token_t *ident, *t;
    asnode_t *left;

    next(&ident);
    ident->token = ST_CALL;
    ident->val.id = findglob(*(char **)names->get[ident->val.id]);

    next(&t); // (
    left = explist();
    next(&t); // )

    if (semi)
    {
        next(&t); // ;
    }

    return mknode(ident, left, NULL, NULL);
}

asnode_t *ret_stmt()
{
    token_t *ret, *t;
    asnode_t *left = NULL;

    next(&ret); // return

    next(&t);
    if (t->token != T_SEMICOLON)
    {
        back();
        left = binexp(0); // expression
        next(&t);         // ;
    }

    ret->token = ST_RETURN;
    return mknode(ret, left, NULL, NULL);
}

asnode_t *block_stmt(sym_t *func)
{
    token_t *t;
    asnode_t *right, *root = NULL;

    token_t *join = malloc(sizeof(token_t));
    join->token = ST_JOIN;

    next(&t);

    while (next(&t))
    {
        // printf("stmt | %d %d\n", t->line, t->token);
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
            var_decl(C_LOCL);
            break;
        case T_IDENT:
            if (tokseq(2, T_IDENT, T_LPAR))
            {
                right = func_call(1);
            }
            else
            {
                right = binexp(0); // assignment expression
                next(&t);          // ;
            }
            break;
        case T_IF:
            right = cond_stmt();
            break;
        case T_WHILE:
            right = cond_stmt();
            break;
        case T_RETURN:
            right = ret_stmt();
            break;
        case T_RBRACE:
            next(&t);
            return root;
        default:
            right = binexp(0); // assignment expression
            next(&t);          // ;
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
