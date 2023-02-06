#include "includes.h"

int main(int argc, char *argv[])
{
    srcf = fopen(argv[1], "r");
    asmf = fopen("out.asm", "w");

    lex();

    // printf("D: %d\n", findglob("abc"));

    asm_preamble();
    asm_postamble();

    fclose(srcf);
    fclose(asmf);

    /*lex();

    asnode_t *root = binexp(0);

    asm_preamble();
    gen(root);
    asm_postamble();

    fclose(srcf);
    fclose(asmf);*/

    /*lex();
    printf("token: %d\n", ((token_t *)tokens->get[0])->token);
    printf("token: %d\n", ((token_t *)tokens->get[1])->token);
    printf("token: %d\n", ((token_t *)tokens->get[2])->token);*/

    return 0;
}