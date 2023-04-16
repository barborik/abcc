#include "includes.h"

// make (construct) an asnode
Node *mknode(int token, Node *left, Node *mid, Node *right)
{
    Node *node = malloc(sizeof(Node));

    node->flags = 0b00000001;
    node->token.token = token;
    node->left = left;
    node->mid = mid;
    node->right = right;

    // setrval(node, 1);

    return node;
}

Node *mkleaf(Tok *token, int flags)
{
    Node *node = malloc(sizeof(Node));

    node->flags = flags;
    node->token = *token;
    node->left = NULL;
    node->mid = NULL;
    node->right = NULL;

    // setrval(node, 1);

    return node;
}

// checks if a given token is an operator token
int isop(int token)
{
    if (token > LT_OP_START && token < LT_OP_END ||
        token > ST_OP_START && token < ST_OP_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a type token
int istype(int token)
{
    if (token > LT_TYPE_START && token < LT_TYPE_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a literal token
int isliteral(int token)
{
    if (token > LT_LIT_START && token < LT_LIT_END ||
        token > ST_LIT_START && token < ST_LIT_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is an identifier token
int isident(int token)
{
    if (token == LT_IDENT || token == ST_IDENT)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a block token
int isblock(int token)
{
    if (token > ST_BLOCK_START && token < ST_BLOCK_END)
    {
        return 1;
    }
    return 0;
}

int rassoc(int token)
{
    if (token == LT_ASSIGN || token == ST_ASSIGN)
    {
        return 1;
    }
    return 0;
}

int getrval(Node *node)
{
    return node->flags & 0b00000001;
}

void setrval(Node *node, int val)
{
    (val) ? (node->flags |= 0b00000001) : (node->flags &= 0b11111110);
}

int interpret(Node *root)
{
    int left, right;
    Tok *t = &root->token;

    if (root->left)
    {
        left = interpret(root->left);
    }
    if (root->right)
    {
        right = interpret(root->right);
    }

    switch (t->token)
    {
    case ST_ADD:
        return left + right;
    case ST_SUB:
        return left - right;
    case ST_MUL:
        return left * right;
    case ST_DIV:
        return left / right;
    case ST_INTLIT:
        return t->val.i;
    case ST_CHARLIT:
        return t->val.i;
    }

    printf("ERROR: line %d token %d\n", t->line, t->token);
    exit(1);
}
