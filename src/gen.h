#ifndef __GEN_
#define __GEN_

enum
{
    A_WALK,
    A_EXEC,
    A_ARGS,
};

int gen(Node *root, int reg, int cmd);

#endif