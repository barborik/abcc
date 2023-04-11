#include "includes.h"

int main(int argc, char *argv[])
{
    src_f = fopen(argv[1], "r");
    out_f = fopen(argv[2], "w");

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

    // asnode_t *root = stmt();
    // gen(root, NULLREG);
    decl();

    // find main and generate it
    int skip;
    for (int i = 0; i < glob->used; i++)
    {
        Sym *sym = glob->get[i];
        if (!strcmp(sym->name, "main"))
        {
            skip = i;
            func = sym;
            gen(sym->root, NULLREG, A_WALK);
            break;
        }
    }

    // generate the rest
    for (int i = 0; i < glob->used; i++)
    {
        if (i == skip)
        {
            continue;
        }

        Sym *sym = glob->get[i];
        if (sym->root)
        {
            func = sym;
            gen(sym->root, NULLREG, A_WALK);
        }
    }

    asm_postamble();

    fclose(src_f);
    fclose(out_f);

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