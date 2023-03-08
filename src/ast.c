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

int bin2st(int tok)
{
    switch (tok)
    {
    case T_PLUS:
        return ST_ADD;
    case T_MINUS:
        return ST_SUB;
    case T_ASTERISK:
        return ST_MUL;
    case T_FSLASH:
        return ST_DIV;
    case T_EQ:
        return ST_EQ;
    case T_NE:
        return ST_NE;
    case T_GT:
        return ST_GT;
    case T_LT:
        return ST_LT;
    case T_GE:
        return ST_GE;
    case T_LE:
        return ST_LE;
    case T_AMP:
        return ST_AND;
    case T_PIPE:
        return ST_OR;
    case T_INTLIT:
        return ST_INTLIT;
    case T_IDENT:
        return ST_IDENT;
    }

    return -1;
}

int un2st(int tok)
{
    switch (tok)
    {
    case T_EXCL:
        return ST_NOT;
    case T_AMP:
        return ST_ADDR;
    case T_ASTERISK:
        return ST_DEREF;
    }

    return -1;
}

int stmt2st(int tok)
{
    switch (tok)
    {
    case T_ASSIGN:
        return ST_ASSIGN;
    case T_IF:
        return ST_IF;
    case T_WHILE:
        return ST_WHILE;
    }

    return -1;
}

// checks if a given token is an operator token
int isop(token_t *t)
{
    if (t->token > OP_START && t->token < OP_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a type token
int istype(token_t *t)
{
    if (t->token > TYPE_START && t->token < TYPE_END)
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
    token_l->token = bin2st(token_l->token);
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
        op->token = bin2st(op->token);

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
