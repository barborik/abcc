#ifndef __AST_
#define __AST_

struct node
{
    /*
    bit
    0 - rvalue
    */
    int flags;
    Tok token;
    struct node *left;
    struct node *mid;
    struct node *right;
};

int getrval(Node *node);
void setrval(Node *node, int val);

int isop(int token);
int istype(int token);
int isblock(int token);

Node *binexp(int ptp);
Node *unexp();

Node *mknode(int token, Node *left, Node *mid, Node *right);
Node *mkleaf(Tok *token, int flags);

#endif