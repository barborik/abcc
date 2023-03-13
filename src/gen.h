#ifndef __GEN_
#define __GEN_

#define NULLREG -1

extern FILE *asmf;

enum
{
    A_WALK,
    A_EXEC,
};

void asm_preamble();
void asm_postamble();
int gen(asnode_t *root, int reg, int cmd);

#endif