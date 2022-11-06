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

asnode_t *binexp(int ptp);
int interpret(asnode_t *root);

#endif