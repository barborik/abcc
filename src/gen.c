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

void rfree(int reg)
{
    regalloc[reg] = 0;
}

void rfree_all()
{
    memset(regalloc, 0, sizeof(regalloc));
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

void asm_addglob(sym_t *sym, size_t val)
{
    char len[4];
    if (sym->type == T_I8)  sprintf(len, "db");
    if (sym->type == T_I16) sprintf(len, "dw");
    if (sym->type == T_I32) sprintf(len, "dd");
    if (sym->type == T_I64) sprintf(len, "dq");

    fprintf(asmf, "%s:\t\t%s %zu\n", sym->name, len, val);
}

int asm_loadglob(sym_t *sym)
{
    int reg = ralloc();
    fprintf(asmf, "\tmov\t%s, [%s]\n", reglist[reg], sym->name);
    return reg;
}

int asm_storeglob(int reg, sym_t *sym)
{
    fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reglist[reg]);
    return reg;
}

void asm_preamble()
{
    // externs, globals etc.
    fprintf(asmf,
            "\tglobal  main\n"
            "\textern  printf\n"
            "\textern  exit\n\n");

    // bss section
    fprintf(asmf,
            "\tsection .bss\n");
    for (size_t i = 0; glob && i < glob->used; i++)
    {
        asm_addglob(glob->get[i], 0);
    }

    // text section
    fprintf(asmf,
            "\n\tsection .text\n"
            "format:\n"
            "\tdb\t\t\"%%d\", 10, 0\n"
            "printint:\n"
            "\tmov\t\trsi, rax\n"
            "\tmov\t\trdi, format\n"
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
            "\tmov\t\trsi, 0\n"
            "\tsub\t\trsp, 32\n"
            "\tcall\texit\n");
}

int gen(asnode_t *root, int reg)
{
    int leftreg = NULLREG, rightreg = NULLREG;

    if (root->token->token != T_JOIN)
    {
        if (root->left)
        {
            leftreg = gen(root->left, -1);
        }
        if (root->right)
        {
            rightreg = gen(root->right, leftreg);
        }
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
        return asm_loadglob(glob->get[root->token->value.id]);
    case T_LVIDENT:
        return asm_storeglob(reg, glob->get[root->token->value.id]);
    case T_ASSIGN:
        return rightreg;
    case T_JOIN:
        gen(root->left, NULLREG);
        rfree_all();
        gen(root->right, NULLREG);
        rfree_all();
        return NULLREG;
    default:
        printf("ERROR: unknown operator %d\n", root->token->token);
        exit(1);
    }
}