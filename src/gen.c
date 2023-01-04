#include "includes.h"

FILE *asmf;

int regalloc[] = {0, 0, 0, 0};

int ralloc()
{
    for (int i = 0; i < 4; i++)
    {
        if (!regalloc[i])
        {
            regalloc[i] = 1;
            return i;
        }
    }

    return -1;
}

int rfree(int reg)
{
    regalloc[reg] = 0;
}

int asm_load(int val)
{
    int reg = ralloc();
    fprintf(asmf, "\tmov\t\t%s, %d\n", reglist[reg], val);
    return reg;
}

int asm_add(int reg0, int reg1)
{
    fprintf(asmf, "\tadd\t\t%s, %s\n", reglist[reg0], reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_sub(int reg0, int reg1)
{
    fprintf(asmf, "\tsub\t\t%s, %s\n", reglist[reg0], reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_mul(int reg0, int reg1)
{
    fprintf(asmf, "\timul\t%s, %s\n", reglist[reg0], reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_div(int reg0, int reg1)
{
    fprintf(asmf, "\tmov\t\trax, %s\n", reglist[reg0]);
    fprintf(asmf, "\tcqo\n");
    fprintf(asmf, "\tidiv\t%s\n", reglist[reg1]);
    fprintf(asmf, "\tmov\t\t%s, rax\n", reglist[reg0]);
    rfree(reg1);
    return reg0;
}

void asm_addglob(token_t *t, size_t val)
{
    char len[8];
    if (t->token == T_I8)  sprintf(len, "db");
    if (t->token == T_I16) sprintf(len, "dw");
    if (t->token == T_I32) sprintf(len, "dd");
    if (t->token == T_I64) sprintf(len, "dq");

    fprintf(asmf, "%s:\t\t%s %zu\n", glob->get[t->value.id], len, val);
}

int asm_loadglob(int id)
{
    int reg = ralloc();
    fprintf(asmf, "\tmov\t%s, [%s]\n", reglist[reg], glob->get[id]);
    return reg;
}

void asm_preamble()
{
    // externs, globals etc.
    fprintf(asmf,
            "\tglobal  main\n"
            "\textern  printf\n"
            "\textern  ExitProcess\n\n");

    // bss section
    fprintf(asmf, 
            "\tsection .bss\n"
    );
    token_t t;
    while(next(&t))
    {
        if(istype(&t))
        {
            asm_addglob(&t, 0);
        }
    }
    reset();

    // text section
    fprintf(asmf,
            "\n\tsection .text\n"
            "format:\n"
            "\tdb\t\t\"%%d\", 10, 0\n"
            "printint:\n"
            "\tmov\t\trdx, rax\n"
            "\tmov\t\trcx, format\n"
            "\tsub\t\trsp, 32\n"
            "\tcall\tprintf\n"
            "\tadd\t\trsp, 32\n"
            "\tret\n\n"
            "main:\n");
}

void asm_postamble()
{
    fprintf(asmf,
            "\n\tcall printint\n"
            "\tmov\t\trcx, 0\n"
            "\tsub\t\trsp, 32\n"
            "\tcall\tExitProcess\n");
}

int gen(asnode_t *root)
{
    int leftreg, rightreg;

    if (root->left)
    {
        leftreg = gen(root->left);
    }
    if (root->right)
    {
        rightreg = gen(root->right);
    }

    switch (root->token->token)
    {
    case T_PLUS:
        return asm_add(leftreg, rightreg);
    case T_MINUS:
        return asm_sub(leftreg, rightreg);
    case T_ASTERISK:
        return asm_mul(leftreg, rightreg);
    case T_FSLASH:
        return asm_div(leftreg, rightreg);
    case T_INTLIT:
        return asm_load(root->token->value.i);
    case T_IDENT:
        return asm_loadglob(root->token->value.id);
    default:
        printf("ERROR: unknown operator %d\n", root->token->token);
        exit(1);
    }
}