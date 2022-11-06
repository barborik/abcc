#ifndef __GEN_
#define __GEN_

extern FILE *asmf;

void asm_preamble();
void asm_postamble();
int gen(asnode_t *root);

#endif