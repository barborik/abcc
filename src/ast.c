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
    if (t->token > T_OPSTART && t->token < T_OPEND)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a type token
int istype(token_t *t)
{
    if (t->token > T_TSTART && t->token < T_TEND)
    {
        return 1;
    }
    return 0;
}

// build a binary expression with pratt parsing
asnode_t *binexp(int ptp)
{
    asnode_t *left, *right;
    token_t *op, *token_l;

    // left operand
    next(&token_l);
    left = mknode(token_l, NULL, NULL, NULL);

    // operator (if available)
    if (!next(&op) || !isop(op))
    {
        back();
        return left;
    }

    // right operand + recursively get the rest
    while (prec[op->token] > ptp)
    {
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
