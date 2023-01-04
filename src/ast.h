#ifndef __AST_
#define __AST_

typedef struct asnode
{
    token_t *token;
    struct asnode *left;
    struct asnode *right;
} asnode_t;

typedef struct
{
    asnode_t *root;
} astree_t;

int isop(token_t *t);
int istype(token_t *t);
asnode_t *binexp(int ptp);
asnode_t *mknode(token_t *token, asnode_t *left, asnode_t *right);

int interpret(asnode_t *root);

#endif