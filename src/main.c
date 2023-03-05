#include "includes.h"

int main(int argc, char *argv[])
{
    srcf = fopen(argv[1], "r");
    asmf = fopen(argv[2], "w");

    lex();

    /*token_t t;
    while (next(&t)) printf("%d\n", t.token);
    return;*/

    asm_preamble();

    // var_decl();
    // assign_stmt();
    // asnode_t *root = stmt();

    // printf("left: %d right: %d\n", root->left->token->token, root->right->token->token);

    // gen(root, -1);
    /*token_t *t;
    while (next(&t))
    {
        printf("token: %d\n", t->token);
    }*/
    asnode_t *root = stmt();
    gen(root, NULLREG);

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