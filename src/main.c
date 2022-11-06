#include "includes.h"

int main(int argc, char *argv[])
{
    srcf = fopen(argv[1], "r");
    asmf = fopen("out.asm", "w");

    lex();

    asnode_t *root = binexp(0);

    asm_preamble();
    gen(root);
    asm_postamble();

    fclose(srcf);
    fclose(asmf);

    return 0;
}