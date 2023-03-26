#ifndef __AST_
#define __AST_

typedef struct asnode
{
    int type;
    /*
    bit
    0 - rvalue
    */
    int flags;
    token_t *token;
    struct asnode *left;
    struct asnode *mid;
    struct asnode *right;
} asnode_t;

int getrval(asnode_t *node);
void setrval(asnode_t *node, int val);

int isop(int token);
int istype(int token);
int isvaltype(int token);
int isptr(int token);
int isdptr(int token);
int isblock(int token);
asnode_t *binexp(int ptp);
asnode_t *unexp();
asnode_t *mknode(token_t *token, asnode_t *left, asnode_t *mid, asnode_t *right);

#endif