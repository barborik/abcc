#ifndef __GEN_
#define __GEN_

#define NULLREG -1

extern FILE *asmf;

enum
{
    A_WALK,
    A_EXEC,
};

void rfree_all();
void asm_preamble();
void asm_postamble();
void asm_addglob(sym_t *sym, size_t val);
int gen(asnode_t *root, int reg, int cmd);

#endif