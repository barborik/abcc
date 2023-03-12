#ifndef __AST_
#define __AST_

typedef struct asnode
{
    int type;
    token_t *token;
    struct asnode *left;
    struct asnode *mid;
    struct asnode *right;
} asnode_t;

typedef struct
{
    asnode_t *root;
} astree_t;

int isop(token_t *t);
int istype(token_t *t);
int isvaltype(token_t *t);
int isptr(token_t *t);
int isdptr(token_t *t);
int isblock(token_t *t);
asnode_t *binexp(int ptp);
asnode_t *mknode(token_t *token, asnode_t *left, asnode_t *mid, asnode_t *right);

#endif