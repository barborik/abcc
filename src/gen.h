#ifndef __GEN_
#define __GEN_

#define NULLREG -1

enum
{
    A_WALK,
    A_EXEC,
    A_ARGS,
};

void asm_preamble();
void asm_postamble();
int gen(Node *root, int reg, int cmd);

#endif