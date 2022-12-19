#include "includes.h"

// make (construct) an asnode
asnode_t *mknode(token_t *token, asnode_t *left, asnode_t *right)
{
    asnode_t *node = malloc(sizeof(asnode_t));

    node->token = token;
    node->left = left;
    node->right = right;

    return node;
}

// checks if a given token is an operator
int isop(token_t t)
{
    if (t.token > T_OPSTART && t.token < T_OPEND)
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
    token_l = malloc(sizeof(token_t));
    next(token_l);
    left = mknode(token_l, NULL, NULL);

    // operator (if available)
    op = malloc(sizeof(token_t));
    if (!next(op) || !isop(*op))
    {
        back();
        free(op);
        return left;
    }

    // right operand + recursively get the rest
    while (prec[op->token] > ptp)
    {
        right = binexp(prec[op->token]);
        left = mknode(op, left, right);

        op = malloc(sizeof(token_t));
        if (!next(op) || !isop(*op))
        {
            back();
            free(op);
            return left;
        }
    }

    back();
    return left;
}

// interprets the AST for testing purposes
int interpret(asnode_t *root)
{
    int leftop, rightop;

    if (root->left)
    {
        leftop = interpret(root->left);
    }
    if (root->right)
    {
        rightop = interpret(root->right);
    }

    switch (root->token->token)
    {
    case T_PLUS:
        return leftop + rightop;
    case T_MINUS:
        return leftop - rightop;
    case T_ASTERISK:
        return leftop * rightop;
    case T_FSLASH:
        return leftop / rightop;
    case T_INTLIT:
        return root->token->value.i;
    default:
        printf("ERROR: unknown operator %d\n", root->token->token);
        exit(1);
    }
}
