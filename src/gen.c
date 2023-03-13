#include "includes.h"

FILE *asmf;

int regalloc[] = {0, 0, 0, 0};
int label = 1;

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

int asm_lognot(int reg)
{
    int tmp = ralloc();
    fprintf(asmf, "\txor\t\t%s, %s\n", reglist[tmp], reglist[tmp]);
    fprintf(asmf, "\ttest\t%s, %s\n", reglist[reg], reglist[reg]);
    fprintf(asmf, "\tsetz\t%s\n", reglist8[tmp]);
    fprintf(asmf, "\tmov\t\t%s, %s\n", reglist[reg], reglist[tmp]);
    rfree(tmp);
    return reg;
}

int asm_bitnot(int reg)
{
    fprintf(asmf, "\tnot\t\t%s\n", reglist[reg]);
    return reg;
}

int asm_addr(sym_t *sym)
{
    int reg = ralloc();
    fprintf(asmf, "\tmov\t\t%s, %s\n", reglist[reg], sym->name);
    return reg;
}

int asm_deref(int reg)
{
    fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist[reg], reglist[reg]);
    return reg;
}

void asm_addglob(sym_t *sym)
{
    char len[8];

    switch (sym->type)
    {
    case T_I8:
    case T_U8:
        sprintf(len, "resb");
        break;
    case T_I16:
    case T_U16:
        sprintf(len, "resw");
        break;
    case T_I32:
    case T_U32:
        sprintf(len, "resd");
        break;
    case T_I64:
    case T_U64:
        sprintf(len, "resq");
        break;
    default:
        sprintf(len, "resq");
    }

    fprintf(asmf, "%s:\t\t\t\t%s 1\n", sym->name, len);
}

int asm_loadglob(sym_t *sym)
{
    int reg = ralloc();
    fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist[reg], sym->name);
    return reg;
}

int asm_storeglob(int reg, sym_t *sym)
{
    fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reglist[reg]);
    return reg;
}

void asm_label(int l)
{
    fprintf(asmf, "L%d:\n", l);
}

int asm_cmpset(int reg0, int reg1, char *ins)
{
    fprintf(asmf, "\tcmp\t\t%s, %s\n", reglist[reg0], reglist[reg1]);
    fprintf(asmf, "\t%s\t%s\n", ins, reglist8[reg0]);
    fprintf(asmf, "\tand\t\t%s, 0xff\n", reglist[reg0]);
    return reg0;
}

void asm_cmpz(int reg)
{
    fprintf(asmf, "\tcmp\t\t%s, 0\n", reglist[reg]);
}

void asm_jumpeq(int l)
{
    fprintf(asmf, "\tje\t\tL%d\n", l);
}

void asm_jump(int l)
{
    fprintf(asmf, "\tjmp\t\tL%d\n", l);
}

void asm_func(asnode_t *root, sym_t *sym)
{
    fprintf(asmf, "%s:\n", sym->name);
    gen(root, NULLREG, A_EXEC);

    if (!strcmp(sym->name, "main"))
    {
        fprintf(asmf,
                "\tmov\t\trsi, 0\n"
                "\tsub\t\trsp, 32\n"
                "\tcall\texit\n\n");
    }
    else
    {
        fprintf(asmf, "\tret\n");
    }
}

void asm_preamble()
{
    // externs, globals etc.
    fprintf(asmf,
            "\tglobal  main\n"
            "\textern  exit\n\n");

    // text section
    fprintf(asmf, "\tsection .text\n");
}

void asm_postamble()
{
    // bss section
    fprintf(asmf,
            "\n\tsection .bss\n");
    for (size_t i = 0; glob && i < glob->used; i++)
    {
        sym_t *sym = glob->get[i];
        if (!sym->func)
        {
            asm_addglob(sym);
        }
    }
}

int gen(asnode_t *root, int reg, int cmd)
{
    int start, end;
    int leftreg = NULLREG, rightreg = NULLREG;

    token_t *t = root->token;
    if (t->token != ST_JOIN && !isblock(t) && cmd == A_EXEC)
    {
        if (root->left)
        {
            leftreg = gen(root->left, NULLREG, cmd);
        }
        if (root->right)
        {
            rightreg = gen(root->right, leftreg, cmd);
        }
    }

    switch (root->token->token)
    {
    case ST_ADD:
        return asm_add(leftreg, rightreg);
    case ST_SUB:
        return asm_sub(leftreg, rightreg);
    case ST_MUL:
        return asm_mul(leftreg, rightreg);
    case ST_DIV:
        return asm_div(leftreg, rightreg);
    case ST_INTLIT:
        return asm_load(root->token->value.i);
    case ST_IDENT:
        return asm_loadglob(glob->get[root->token->value.id]);
    case ST_LVIDENT:
        return asm_storeglob(reg, glob->get[root->token->value.id]);
    case T_ASSIGN:
        return rightreg;
    case ST_JOIN:
        gen(root->left, NULLREG, cmd);
        rfree_all();
        gen(root->right, NULLREG, cmd);
        rfree_all();
        return NULLREG;
    case ST_EQ:
        return asm_cmpset(leftreg, rightreg, "sete");
    case ST_NE:
        return asm_cmpset(leftreg, rightreg, "setne");
    case ST_GT:
        return asm_cmpset(leftreg, rightreg, "setg");
    case ST_LT:
        return asm_cmpset(leftreg, rightreg, "setl");
    case ST_GE:
        return asm_cmpset(leftreg, rightreg, "setge");
    case ST_LE:
        return asm_cmpset(leftreg, rightreg, "setle");
    case ST_LOGNOT:
        return asm_lognot(leftreg);
    case ST_BITNOT:
        return asm_bitnot(leftreg);
    case ST_ADDR:
        return asm_addr(glob->get[root->left->token->value.id]);
    case ST_DEREF:
        return asm_deref(leftreg);
    case ST_IF:
        rightreg = gen(root->mid, NULLREG, cmd);
        asm_cmpz(rightreg);
        end = label++;
        asm_jumpeq(end);
        if (root->left)
        {
            gen(root->left, NULLREG, cmd);
        }
        asm_label(end);
        return NULLREG;
    case ST_WHILE:
        start = label++;
        asm_label(start);
        rightreg = gen(root->mid, NULLREG, cmd);
        asm_cmpz(rightreg);
        end = label++;
        asm_jumpeq(end);
        if (root->left)
        {
            gen(root->left, NULLREG, cmd);
        }
        asm_jump(start);
        asm_label(end);
        return NULLREG;
    case ST_FUNC:
        asm_func(root->left, glob->get[root->token->value.id]);
        return NULLREG;
    default:
        printf("ERROR: unknown token %d on line %d\n", root->token->token, root->token->line);
        exit(1);
    }
}