#include "includes.h"

FILE *asmf;
int label = 1;
sym_t *sym;  // last symbol
sym_t *func; // current function

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

int asm_addr(token_t *t)
{
    sym_t *sym;
    int reg = ralloc();

    if (t->class == C_LOCL)
    {
        sym = func->local->get[t->val.id];
        fprintf(asmf, "\tmov\t\t%s, rsp\n", reglist[reg]);
        fprintf(asmf, "\tadd\t\t%s, %d\n", reglist[reg], sym->offs);
    }
    else
    {
        sym = glob->get[t->val.id];
        fprintf(asmf, "\tmov\t\t%s, %s\n", reglist[reg], sym->name);
    }

    return reg;
}

int asm_deref(int reg, int type)
{
    switch (type)
    {
    case T_U8:
    case T_I8:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist8[reg], reglist[reg]);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist16[reg], reglist[reg]);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist32[reg], reglist[reg]);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist64[reg], reglist[reg]);
        break;
    default:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist64[reg], reglist[reg]);
        break;
    }

    return reg;
}

void asm_addglob(sym_t *sym)
{
    fprintf(asmf, "%s:\t\t\t\tresb %d\n", sym->name, type2size(sym->type) * sym->size);
}

int asm_loadglob(sym_t *sym)
{
    int reg = ralloc();

    switch (sym->type)
    {
    // value types
    case T_U8:
    case T_I8:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist8[reg], sym->name);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist16[reg], sym->name);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist32[reg], sym->name);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist64[reg], sym->name);
        break;

    // pointers
    default:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reglist64[reg], sym->name);
        break;
    }

    return reg;
}

int asm_storeglob(int reg, sym_t *sym)
{
    switch (sym->type)
    {
    case T_U8:
    case T_I8:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reglist8[reg]);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reglist16[reg]);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reglist32[reg]);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reglist64[reg]);
        break;
    default:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reglist64[reg]);
        break;
    }
    
    return reg;
}

int asm_loadlocl(sym_t *sym)
{
    int reg = ralloc();

    switch (sym->type)
    {
    // value types
    case T_U8:
    case T_I8:
        fprintf(asmf, "\tmov\t\t%s, [rsp + %d]\n", reglist8[reg], sym->offs);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t%s, [rsp + %d]\n", reglist16[reg], sym->offs);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t%s, [rsp + %d]\n", reglist32[reg], sym->offs);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t%s, [rsp + %d]\n", reglist64[reg], sym->offs);
        break;

    // pointers
    default:
        fprintf(asmf, "\tmov\t\t%s, [rsp + %d]\n", reglist64[reg], sym->offs);
        break;
    }

    return reg;
}

int asm_storelocl(int reg, sym_t *sym)
{
    switch (sym->type)
    {
    case T_U8:
    case T_I8:
        fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist8[reg]);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist16[reg]);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist32[reg]);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist64[reg]);
        break;
    default:
        fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist64[reg]);
        break;
    }

    return reg;
}

int asm_storederef(int reg0, int reg1, sym_t *sym)
{
    switch (sym->type)
    {
    case T_U8:
    case T_I8:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reglist[reg1], reglist8[reg0]);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reglist[reg1], reglist16[reg0]);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reglist[reg1], reglist32[reg0]);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reglist[reg1], reglist64[reg0]);
        break;
    default:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reglist[reg1], reglist64[reg0]);
        break;
    }
    return reg0;
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

int asm_func(asnode_t *root)
{
    fprintf(asmf, "%s:\n", func->name);

    for (int i = 0; i < func->argc; i++)
    {
        sym_t *sym = func->local->get[i];
        fprintf(asmf, "\tsub\t\trsp, %d\n", type2size(sym->type));
        switch (sym->type)
        {
        case T_I8:
        case T_U8:
            fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist8[i]);
            break;
        case T_I16:
        case T_U16:
            fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist16[i]);
            break;
        case T_I32:
        case T_U32:
            fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist32[i]);
            break;
        case T_I64:
        case T_U64:
            fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist64[i]);
            break;
        default:
            fprintf(asmf, "\tmov\t\t[rsp + %d], %s\n", sym->offs, reglist64[i]);
            break;
        }
        sym->level = 1;
        dl_add(func->stack, &sym);
    }

    gen(root, NULLREG, A_EXEC);
    return NULLREG;
}

int asm_alloc(sym_t *sym)
{
    for (int i = 0; i < func->stack->used; i++)
    {
        sym_t *tmp = *(sym_t **)func->stack->get[i];
        tmp->offs += type2size(sym->type) * sym->size;
    }

    dl_add(func->stack, &sym);
    fprintf(asmf, "\tsub\t\trsp, %d\n", type2size(sym->type) * sym->size);
    return NULLREG;
}

int asm_call(asnode_t *args, sym_t *sym)
{
    int reg, align = 0;

    rfree_all();

    gen(args, NULLREG, A_ARGS);

    rfree_all();

    /*for (int i = 0; i < NREGS; i++)
    {
        if (regalloc[i])
        {
            fprintf(asmf, "\tpush\t%s\n", reglist[i]);
        }
    }*/

    /*for (int i = 0; i < func->local->used; i++)
    {
        sym_t *sym = func->local->get[i];
        offs += sym->offs;
    }*/

    for (int i = 0; i < func->stack->used; i++)
    {
        sym_t *sym = func->local->get[i];
        align += type2size(sym->type) * sym->size;
    }

    align = 8 - align;
    if (align < 0)
    {
        align = -align - 8;
    }

    fprintf(asmf, "\tsub\t\trsp, %d\n", align);
    fprintf(asmf, "\tcall\t%s\n", sym->name);
    fprintf(asmf, "\tadd\t\trsp, %d\n", align);

    if (sym->type != T_U0 && sym->type != T_I0)
    {
        reg = ralloc();
        fprintf(asmf, "\tmov\t\t%s, rax\n", reglist[reg]);
    }

    /*for (int i = NREGS - 1; i >= 0; i--)
    {
        if (i == reg)
        {
            continue;
        }

        if (regalloc[i])
        {
            fprintf(asmf, "\tpop\t\t%s\n", reglist[i]);
        }
    }*/

    if (func->type == T_U0 || func->type == T_I0)
    {
        return NULLREG;
    }
    return reg;
}

void asm_stackfree()
{
    int add = 0;
    for (int i = 0; i < func->stack->used; i++)
    {
        sym_t *sym = *(sym_t **)func->stack->get[i];
        if (sym->level > func->level)
        {
            add += type2size(sym->type) * sym->size;
            dl_rem(func->stack, i);
            i--;
        }
    }
    if (add)
    {
        fprintf(asmf, "\tadd\t\trsp, %d\n", add);
    }
}

int asm_ret(int reg)
{
    func->level--;
    asm_stackfree();
    if (reg != NULLREG)
    {
        fprintf(asmf, "\tmov\t\trax, %s\n", reglist[reg]);
    }
    fprintf(asmf, "\tret\n\n");
    return NULLREG;
}

int asm_if(asnode_t *root, int cmd)
{
    int end, reg;
    reg = gen(root->mid, NULLREG, cmd);
    asm_cmpz(reg);
    end = label++;
    asm_jumpeq(end);
    rfree_all();
    if (root->left)
    {
        func->level++;
        gen(root->left, NULLREG, cmd);
        func->level--;
    }
    asm_label(end);
    return NULLREG;
}

int asm_while(asnode_t *root, int cmd)
{
    int start, end, reg;
    start = label++;
    asm_label(start);
    reg = gen(root->mid, NULLREG, cmd);
    asm_cmpz(reg);
    end = label++;
    asm_jumpeq(end);
    rfree_all();
    if (root->left)
    {
        gen(root->left, NULLREG, cmd);
    }
    asm_jump(start);
    asm_label(end);

    return NULLREG;
}

void asm_preamble()
{
    // main directive
    fprintf(asmf, "\tglobal  main\n\n");

    // text section
    fprintf(asmf, "\tsection .text\n");
}

void asm_postamble()
{
    // externs
    for (int i = 0; i < glob->used; i++)
    {
        sym_t *sym = glob->get[i];
        if (sym->class == C_EXTN)
        {
            fprintf(asmf, "\textern %s\n", sym->name);
        }
    }

    // bss section
    fprintf(asmf,
            "\n\tsection .bss\n");
    for (size_t i = 0; glob && i < glob->used; i++)
    {
        sym_t *sym = glob->get[i];
        if (sym->class == C_GLOB)
        {
            asm_addglob(sym);
        }
    }
}

int gen(asnode_t *root, int reg, int cmd)
{
    reglist = reglist64_elf64;
    reglist64 = reglist64_elf64;
    reglist32 = reglist32_elf64;
    reglist16 = reglist16_elf64;
    reglist8 = reglist8_elf64;
    regalloc = regalloc_elf64;

    token_t *t;
    int leftreg = NULLREG, rightreg = NULLREG;

    if (!root)
    {
        return;
    }

    t = root->token;

    if (t->token == ST_CALL)
    {
        cmd = A_WALK;
    }

    if (t->token != ST_JOIN && !isblock(t) && cmd != A_WALK)
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

    asm_stackfree();

    // DEBUG
    fprintf(asmf, "\t; line %d\n", t->line);

    switch (t->token)
    {
    case ST_ADD: return asm_add(leftreg, rightreg);
    case ST_SUB: return asm_sub(leftreg, rightreg);
    case ST_MUL: return asm_mul(leftreg, rightreg);
    case ST_DIV: return asm_div(leftreg, rightreg);
    case ST_INTLIT: return asm_load(t->val.i);
    case ST_IDENT:
        switch (t->class)
        {
        case C_LOCL: sym = func->local->get[t->val.id]; break;
        case C_GLOB: sym = glob->get[t->val.id]; break;
        }

        if (getrval(root))
        {
            switch (t->class)
            {
            case C_LOCL: return asm_loadlocl(func->local->get[t->val.id]);
            case C_GLOB: return asm_loadglob(glob->get[t->val.id]);
            }
        }
        return NULLREG;
    case ST_ASSIGN:
        t = root->right->token;
        switch (t->token)
        {
        case ST_IDENT:
            switch (t->class)
            {
            case C_LOCL: return asm_storelocl(leftreg, func->local->get[t->val.id]);
            case C_GLOB: return asm_storeglob(leftreg, glob->get[t->val.id]);
            }
            break;
        case ST_DEREF: return asm_storederef(leftreg, rightreg, sym);
        }
        break;
    case ST_DEREF:
        if (getrval(root))
        {
            return asm_deref(leftreg, root->left->type);
        }
        return leftreg;
    case ST_ADDR: return asm_addr(root->left->token);
    case ST_JOIN:
        gen(root->left, NULLREG, cmd);
        if (cmd != A_ARGS)
        {
            rfree_all();
        }
        gen(root->right, NULLREG, cmd);
        if (cmd != A_ARGS)
        {
            rfree_all();
        }
        return NULLREG;
    case ST_ALLOC: return asm_alloc(func->local->get[t->val.id]);
    case ST_EQ: return asm_cmpset(leftreg, rightreg, "sete");
    case ST_NE: return asm_cmpset(leftreg, rightreg, "setne");
    case ST_GT: return asm_cmpset(leftreg, rightreg, "setg");
    case ST_LT: return asm_cmpset(leftreg, rightreg, "setl");
    case ST_GE: return asm_cmpset(leftreg, rightreg, "setge");
    case ST_LE: return asm_cmpset(leftreg, rightreg, "setle");
    case ST_LOGNOT: return asm_lognot(leftreg);
    case ST_BITNOT: return asm_bitnot(leftreg);
    case ST_IF: return asm_if(root, cmd);
    case ST_WHILE: return asm_while(root, cmd);
    case ST_FUNC: return asm_func(root->left);
    case ST_CALL: return asm_call(root->left, glob->get[t->val.id]);
    case ST_RETURN: return asm_ret(leftreg);
    }

    printf("ERROR: line %d\n", t->line);
    exit(1);
}