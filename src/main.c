#include "includes.h"

void usage(char *name)
{
    printf("usage: %s [flags] <input file(s)> [-o output file(s)]\n\n"
           "flags:\n"

           "\t-h, --help display this help message\n\n"

           "\t-m <mode>\n"
           "\t    64\t\t64-bit mode\n"
           "\t    16\t\t16-bit mode\n\n"

           "\t-cc <calling convention>\n"
           "\t     sysv\tSystemV (Linux, most Unix variants)\n"
           "\t     ms64\tMicrosoft x64 (Windows)\n\n",
           name);
}

int main(int argc, char *argv[])
{
    //usage(argv[0]);
    //exit(0);

    src_n = argv[1];
    src_f = fopen(argv[1], "r");
    out_f = fopen(argv[2], "w");

    prep();

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