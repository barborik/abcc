#include "includes.h"

// make (construct) an asnode
asnode_t *mknode(token_t *token, asnode_t *left, asnode_t *mid, asnode_t *right)
{
    asnode_t *node = malloc(sizeof(asnode_t));

    node->token = token;
    node->left = left;
    node->mid = mid;
    node->right = right;

    return node;
}

// checks if a given token is an operator token
int isop(token_t *t)
{
    if (t->token > T_OP_START && t->token < T_OP_END ||
        t->token > ST_OP_START && t->token < ST_OP_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a type token
int istype(token_t *t)
{
    if (t->token > T_TYPE_START && t->token < T_TYPE_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a value type token
int isvaltype(token_t *t)
{
    if (t->token > T_TYPE_VAL_START && t->token < T_TYPE_VAL_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a pointer token
int isptr(token_t *t)
{
    if (t->token > T_TYPE_PTR_START && t->token < T_TYPE_PTR_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a double pointer token
int isdptr(token_t *t)
{
    if (t->token > T_TYPE_DPTR_START && t->token < T_TYPE_DPTR_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a literal token
int isliteral(token_t *t)
{
    if (t->token > T_LIT_START && t->token < T_LIT_END ||
        t->token > ST_LIT_START && t->token < ST_LIT_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is an identifier token
int isident(token_t *t)
{
    if (t->token == T_IDENT || t->token == ST_IDENT)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a block token
int isblock(token_t *t)
{
    if (t->token > T_BLOCK_START && t->token < T_BLOCK_END ||
        t->token > ST_BLOCK_START && t->token < ST_BLOCK_END)
    {
        return 1;
    }
    return 0;
}

// build an unary expression
asnode_t *unexp()
{
    asnode_t *root;
    token_t *t;

    next(&t);
    un2stx(t);

    root = mknode(t, NULL, NULL, NULL);
    if (!isliteral(t) && !isident(t))
    {
        root->left = unexp();
    }

    return root;
}

// build a binary expression with pratt parsing
asnode_t *binexp(int ptp)
{
    asnode_t *left, *right;
    token_t *op, *token_l;

    // left operand
    // next(&token_l);
    // bin2stx(token_l);
    left = unexp();

    // operator (if available)
    if (!next(&op) || !isop(op))
    {
        back();
        return left;
    }

    // right operand + recursively get the rest
    while (prec[op->token] > ptp)
    {
        bin2stx(op);

        right = binexp(prec[op->token]);
        left = mknode(op, left, NULL, right);

        if (!next(&op) || !isop(op))
        {
            back();
            return left;
        }
    }

    back();
    return left;
}
