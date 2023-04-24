#include "includes.h"

void asm64_stackfree(void)
{
    int add = 0;
    for (int i = 0; i < func->stack->used; i++)
    {
        Sym *sym = *(Sym **)func->stack->get[i];
        if (sym->level > func->level)
        {
            add += type2size(sym->type) * sym->size;
            dl_rem(func->stack, i);
            i--;
        }
    }
    if (add)
    {
        top -= add;
        fprintf(out_f, "\tadd\t\trsp, %d\n", add);
    }
}

int asm64_load(int val)
{
    int reg = ralloc();
    fprintf(out_f, "\tmov\t\t%s, %d\n", reginfo->reglist[reg], val);
    return reg;
}

int asm64_loadstr(int id)
{
    int reg = ralloc();
    fprintf(out_f, "\tmov\t\t%s, S%d\n", reginfo->reglist[reg], id);
    return reg;
}

int asm64_add(int reg0, int reg1)
{
    fprintf(out_f, "\tadd\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm64_sub(int reg0, int reg1)
{
    fprintf(out_f, "\tsub\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm64_mul(int reg0, int reg1)
{
    if (type.addr)
    {
        fprintf(out_f, "\tpush\trax\n");
        fprintf(out_f, "\tmov\t\trax, %s\n", reginfo->reglist[reg0]);
        fprintf(out_f, "\tmul\t\t%s\n", reginfo->reglist[reg1]);
        fprintf(out_f, "\tmov\t\t%s, rax\n", reginfo->reglist[reg0]);
        fprintf(out_f, "\tpop\t\trax\n");
        rfree(reg1);
        return reg0;
    }

    switch (type.type)
    {
    case LT_I8:
    case LT_I16:
    case LT_I32:
    case LT_I64:
        fprintf(out_f, "\timul\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
        break;
    case LT_U8:
    case LT_U16:
    case LT_U32:
    case LT_U64:
        fprintf(out_f, "\tpush\trax\n");
        fprintf(out_f, "\tmov\t\trax, %s\n", reginfo->reglist[reg0]);
        fprintf(out_f, "\tmul\t\t%s\n", reginfo->reglist[reg1]);
        fprintf(out_f, "\tmov\t\t%s, rax\n", reginfo->reglist[reg0]);
        fprintf(out_f, "\tpop\t\trax\n");
        break;
    }
    
    rfree(reg1);
    return reg0;
}

int asm64_div(int reg0, int reg1)
{
    fprintf(out_f, "\tpush\trax\n");
    fprintf(out_f, "\tmov\t\trax, %s\n", reginfo->reglist[reg0]);
    fprintf(out_f, "\tcqo\n");

    if (type.addr)
    {
        fprintf(out_f, "\tdiv\t%s\n", reginfo->reglist[reg1]);
        fprintf(out_f, "\tmov\t\t%s, rax\n", reginfo->reglist[reg0]);
        rfree(reg1);
        return reg0;
    }

    switch (type.type)
    {
    case LT_I8:
    case LT_I16:
    case LT_I32:
    case LT_I64:
        fprintf(out_f, "\tidiv\t%s\n", reginfo->reglist[reg1]);
        break;
    case LT_U8:
    case LT_U16:
    case LT_U32:
    case LT_U64:
        fprintf(out_f, "\tdiv\t%s\n", reginfo->reglist[reg1]);
        break;
    }
    
    fprintf(out_f, "\tmov\t\t%s, rax\n", reginfo->reglist[reg0]);
    fprintf(out_f, "\tpop\t\trax\n");
    rfree(reg1);
    return reg0;
}

int asm64_mod(int reg0, int reg1)
{
    asm64_div(reg0, reg1);
    fprintf(out_f, "\tmov\t\t%s, rdx\n", reginfo->reglist[reg0]);
    return reg0;
}

int asm64_lognot(int reg)
{
    int tmp = ralloc();
    fprintf(out_f, "\txor\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[tmp]);
    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg], reginfo->reglist[reg]);
    fprintf(out_f, "\tsetz\t%s\n", reginfo->reglist8[tmp]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg], reginfo->reglist[tmp]);
    rfree(tmp);
    return reg;
}

int asm64_bitnot(int reg)
{
    if (type.addr)
    {
        fprintf(out_f, "\tnot\t\t%s\n", reginfo->reglist64[reg]);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\tnot\t\t%s\n", reginfo->reglist8[reg]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\tnot\t\t%s\n", reginfo->reglist16[reg]);
        break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\tnot\t\t%s\n", reginfo->reglist32[reg]);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\tnot\t\t%s\n", reginfo->reglist64[reg]);
        break;
    }

    return reg;
}

int asm64_neg(int reg)
{
    if (type.addr)
    {
        fprintf(out_f, "\tneg\t\t%s\n", reginfo->reglist64[reg]);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\tneg\t\t%s\n", reginfo->reglist8[reg]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\tneg\t\t%s\n", reginfo->reglist16[reg]);
        break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\tneg\t\t%s\n", reginfo->reglist32[reg]);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\tneg\t\t%s\n", reginfo->reglist64[reg]);
        break;
    }

    return reg;
}

int asm64_bitor(int reg0, int reg1)
{
    fprintf(out_f, "\tor\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm64_bitand(int reg0, int reg1)
{
    fprintf(out_f, "\tand\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm64_logor(int reg0, int reg1)
{
    int tmp = ralloc();
    fprintf(out_f, "\txor\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[tmp]);

    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg0]);
    fprintf(out_f, "\tsetne\t%s\n", reginfo->reglist8[tmp]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[tmp]);

    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg1], reginfo->reglist[reg1]);
    fprintf(out_f, "\tsetne\t%s\n", reginfo->reglist8[tmp]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg1], reginfo->reglist[tmp]);

    rfree(tmp);
    return asm64_bitor(reg0, reg1);
}

int asm64_logand(int reg0, int reg1)
{
    int tmp = ralloc();
    fprintf(out_f, "\txor\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[tmp]);

    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg0]);
    fprintf(out_f, "\tsetne\t%s\n", reginfo->reglist8[tmp]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[tmp]);

    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg1], reginfo->reglist[reg1]);
    fprintf(out_f, "\tsetne\t%s\n", reginfo->reglist8[tmp]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg1], reginfo->reglist[tmp]);

    rfree(tmp);
    
    return asm64_bitand(reg0, reg1);
}

int asm64_bitxor(int reg0, int reg1)
{
    fprintf(out_f, "\txor\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm64_lshift(int reg0, int reg1)
{
    fprintf(out_f, "\tpush\trcx\n");
    fprintf(out_f, "\tmov\t\trcx, %s\n", reginfo->reglist[reg1]);
    fprintf(out_f, "\tshl\t\t%s, cl\n", reginfo->reglist[reg0]);
    fprintf(out_f, "\tpop\t\trcx\n");
    rfree(reg1);
    return reg0;
}

int asm64_rshift(int reg0, int reg1)
{
    fprintf(out_f, "\tpush\trcx\n");
    fprintf(out_f, "\tmov\t\trcx, %s\n", reginfo->reglist[reg1]);
    fprintf(out_f, "\tshr\t\t%s, cl\n", reginfo->reglist[reg0]);
    fprintf(out_f, "\tpop\t\trcx\n");
    rfree(reg1);
    return reg0;
}

int asm64_addr(Sym *sym)
{
    int reg = ralloc();

    if (sym->class == C_LOCL)
    {
        fprintf(out_f, "\tmov\t\t%s, rbp\n", reginfo->reglist[reg]);
        fprintf(out_f, "\tsub\t\t%s, %d\n", reginfo->reglist[reg], sym->offs);
    }
    else
    {
        fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg], sym->name);
    }

    type.addr++;
    return reg;
}

int asm64_deref(int reg)
{
    type.addr--;

    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t%s, [%s]\n", reginfo->reglist64[reg], reginfo->reglist[reg]);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\tmov\t\t%s, BYTE [%s]\n", reginfo->reglist8[reg], reginfo->reglist[reg]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\tmov\t\t%s, WORD [%s]\n", reginfo->reglist16[reg], reginfo->reglist[reg]);
        break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\tmov\t\t%s, DWORD [%s]\n", reginfo->reglist32[reg], reginfo->reglist[reg]);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\tmov\t\t%s, QWORD [%s]\n", reginfo->reglist64[reg], reginfo->reglist[reg]);
        break;
    }

    return reg;
}

int asm64_loadglob(Sym *sym)
{
    int reg = ralloc();

    if (type.addr || sym->local)
    {
        fprintf(out_f, "\tmov\t\t%s, QWORD %s\n", reginfo->reglist64[reg], sym->name);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:  fprintf(out_f, "\tmovzx\t%s, BYTE [%s]\n", reginfo->reglist32[reg], sym->name); break;
    case LT_I8:  fprintf(out_f, "\tmovsx\t%s, BYTE [%s]\n", reginfo->reglist32[reg], sym->name); break;
    case LT_U16: fprintf(out_f, "\tmovzx\t%s, WORD [%s]\n", reginfo->reglist32[reg], sym->name); break;
    case LT_I16: fprintf(out_f, "\tmovsx\t%s, WORD [%s]\n", reginfo->reglist32[reg], sym->name); break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\tmov\t\t%s, DWORD [%s]\n", reginfo->reglist32[reg], sym->name);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\tmov\t\t%s, QWORD [%s]\n", reginfo->reglist64[reg], sym->name);
        break;
    }

    return reg;
}

int asm64_storeglob(int reg, Sym *sym)
{
    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist64[reg]);
        return reg;
    }

    switch (type.addr)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist8[reg]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist16[reg]);
        break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist32[reg]);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist64[reg]);
        break;
    }
    
    return reg;
}

int asm64_loadlocl(Sym *sym)
{
    int reg = ralloc();

    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t%s, QWORD [rbp - %d]\n", reginfo->reglist64[reg], sym->offs);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:  fprintf(out_f, "\tmovzx\t%s, BYTE [rbp - %d]\n", reginfo->reglist32[reg], sym->offs); break;
    case LT_I8:  fprintf(out_f, "\tmovsx\t%s, BYTE [rbp - %d]\n", reginfo->reglist32[reg], sym->offs); break;
    case LT_U16: fprintf(out_f, "\tmovzx\t%s, WORD [rbp - %d]\n", reginfo->reglist32[reg], sym->offs); break;
    case LT_I16: fprintf(out_f, "\tmovsx\t%s, WORD [rbp - %d]\n", reginfo->reglist32[reg], sym->offs); break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\tmov\t\t%s, DWORD [rbp - %d]\n", reginfo->reglist32[reg], sym->offs);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\tmov\t\t%s, QWORD [rbp - %d]\n", reginfo->reglist64[reg], sym->offs);
        break;
    }

    return reg;
}

int asm64_storelocl(int reg, Sym *sym)
{
    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist64[reg]);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist8[reg]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist16[reg]);
        break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist32[reg]);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist64[reg]);
        break;
    }

    return reg;
}

int asm64_storederef(int reg0, int reg1, Sym *sym)
{
    if (type.addr) type.addr--;

    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist64[reg0]);
        return reg0;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist8[reg0]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist16[reg0]);
        break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist32[reg0]);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist64[reg0]);
        break;
    }
    return reg0;
}

int asm64_push(int reg)
{
    if (type.addr)
    {
        pushed += 8;
        fprintf(out_f, "\tpush\t%s\n", reginfo->reglist[reg]);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        pushed += 4;
        fprintf(out_f, "\tpush\t%s\n", reginfo->reglist32[reg]);
        break;
    case LT_U16:
    case LT_I16:
        pushed += 4;
        fprintf(out_f, "\tpush\t%s\n", reginfo->reglist32[reg]);
        break;
    case LT_U32:
    case LT_I32:
        pushed += 4;
        fprintf(out_f, "\tpush\t%s\n", reginfo->reglist32[reg]);
        break;
    case LT_U64:
    case LT_I64:
        pushed += 8;
        fprintf(out_f, "\tpush\t%s\n", reginfo->reglist64[reg]);
        break;
    }

    return reg;
}

void asm64_label(int l)
{
    fprintf(out_f, "L%d:\n", l);
}

int asm64_cmpset(int reg0, int reg1, char *ins)
{
    if (type.addr)
    {
        fprintf(out_f, "\tcmp\t\t%s, %s\n", reginfo->reglist64[reg0], reginfo->reglist64[reg1]);
        fprintf(out_f, "\t%s\t%s\n", ins, reginfo->reglist8[reg0]);
        fprintf(out_f, "\tand\t\t%s, 0xff\n", reginfo->reglist[reg0]);
        return reg0;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\tcmp\t\t%s, %s\n", reginfo->reglist8[reg0], reginfo->reglist8[reg1]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\tcmp\t\t%s, %s\n", reginfo->reglist16[reg0], reginfo->reglist16[reg1]);
        break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\tcmp\t\t%s, %s\n", reginfo->reglist32[reg0], reginfo->reglist32[reg1]);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\tcmp\t\t%s, %s\n", reginfo->reglist64[reg0], reginfo->reglist64[reg1]);
        break;
    }

    fprintf(out_f, "\t%s\t%s\n", ins, reginfo->reglist8[reg0]);
    fprintf(out_f, "\tand\t\t%s, 0xff\n", reginfo->reglist[reg0]);
    return reg0;
}

void asm64_cmpz(int reg)
{
    fprintf(out_f, "\tcmp\t\t%s, 0\n", reginfo->reglist[reg]);
}

void asm64_jumpeq(int l)
{
    fprintf(out_f, "\tje\t\tL%d\n", l);
}

void asm64_jump(int l)
{
    fprintf(out_f, "\tjmp\t\tL%d\n", l);
}

int asm64_alloc(Sym *sym)
{
    dl_add(func->stack, &sym);
    fprintf(out_f, "\tsub\t\trsp, %d\n", type2size(sym->type) * sym->size);
    
    top += type2size(sym->type) * sym->size;
    sym->offs = top;
    return NULLREG;
}

int asm64_func(Node *root)
{
    top = 0;
    fprintf(out_f, "%s:\n", func->name);
    fprintf(out_f, "\tpush\trbp\n");
    fprintf(out_f, "\tmov\t\trbp, rsp\n");
    
    reginfo = &arg64;

    for (int i = 0; i < func->argc; i++)
    {
        Sym *sym = func->local->get[i];
        type = sym->type;

        asm64_alloc(sym);
        asm64_storelocl(i, sym);
    }

    reginfo = &std64;

    gen(root, NULLREG, A_EXEC);
    return NULLREG;
}

int asm64_call(int reg, Node *args)
{
    RegInfo *tmp;
    int align = 32, offs, *regalloc;
    // 32 bytes ^^ shadow space

    // save the function pointer
    top += 8;
    offs = top;
    fprintf(out_f, "\tpush\t%s\n", reginfo->reglist[reg]);
    /*for (int i = 0; i < func->stack->used; i++)
    {
        Sym *sym = *(Sym **)func->stack->get[i];
        offs += type2size(sym->type) * sym->size;
    }*/

    // push used registers
    for (int i = 0; i < reginfo->nregs; i++)
    {
        if (reginfo->regalloc[i] && i != reg)
        {
            top += 8;
            fprintf(out_f, "\tpush\t%s\n", reginfo->reglist[i]);
        }
    }

    regalloc = malloc(reginfo->nregs * sizeof(int));
    memcpy(regalloc, reginfo->regalloc, reginfo->nregs * sizeof(int));

    tmp = reginfo;
    reginfo = &arg64;
    rfree_all();
    reginfo = tmp;

    tmp = reginfo; 
    rfree_all();
    gen(args, NULLREG, A_ARGS);
    rfree_all();
    reginfo = tmp;

    memcpy(reginfo->regalloc, regalloc, reginfo->nregs * sizeof(int));
    free(regalloc);

    /*for (int i = 0; i < func->stack->used; i++)
    {
        Sym *sym = *(Sym**)func->stack->get[i];
        align += type2size(sym->type) * sym->size;
    }*/
    //align = top % 16;
    align += ALIGN - (top % ALIGN);

    fprintf(out_f, "\txor\t\trax, rax\n");
    if (align) fprintf(out_f, "\tsub\t\trsp, %d\n", align);
    fprintf(out_f, "\tcall\t[rbp - %d]\n", offs);
    if (align) fprintf(out_f, "\tadd\t\trsp, %d\n", align);


    rfree(reg);
    reg = ralloc();
    fprintf(out_f, "\tmov\t\t%s, rax\n", reginfo->reglist[reg]);

    // pop used registers back
    for (int i = reginfo->nregs - 1; i >= 0; i--)
    {
        if (reginfo->regalloc[i] && i != reg)
        {
            top -= 8;
            fprintf(out_f, "\tpop\t\t%s\n", reginfo->reglist[i]);
        }
    }

    top -= 8;
    fprintf(out_f, "\tadd\t\trsp, 8\n");
    
    return reg;
}

int asm64_ret(int reg)
{
    if (reg != NULLREG)
    {
        fprintf(out_f, "\tmov\t\trax, %s\n", reginfo->reglist[reg]);
    }
    fprintf(out_f, "\tleave\n");
    fprintf(out_f, "\tret\n\n");
    return NULLREG;
}

int asm64_break(void)
{
    asm64_jump(end);
    return NULLREG;
}

int asm64_continue(void)
{
    asm64_jump(start);
    return NULLREG;
}

int asm64_if(Node *root, int cmd)
{
    int if_e, else_e, reg;

    if_e = label++;
    if (root->right) else_e = label++;

    func->level++;

    // condition
    reg = gen(root->mid, NULLREG, cmd);
    asm64_cmpz(reg);
    asm64_jumpeq(if_e);

    // if body
    rfree_all();
    gen(root->left, NULLREG, cmd);
    rfree_all();
    if (root->right) asm64_jump(else_e);
    asm64_label(if_e);

    // else body
    if (root->right)
    {
        rfree_all();
        gen(root->right, NULLREG, cmd);
        rfree_all();
        asm64_label(else_e);
    }
    
    func->level--;
    asm64_stackfree();

    return NULLREG;
}

int asm64_while(Node *root, int cmd)
{
    int last_s, last_e, reg;

    // save labels
    last_s = start;
    start = label++;

    last_e = end;
    end = label++;

    func->level++;

    // condition
    asm64_label(start);
    reg = gen(root->mid, NULLREG, cmd);
    asm64_cmpz(reg);

    asm64_jumpeq(end);

    // body
    rfree_all();
    gen(root->left, NULLREG, cmd);
    rfree_all();

    asm64_jump(start);
    asm64_label(end);

    func->level--;
    asm64_stackfree();

    // put labels back
    start = last_s;
    end = last_e;

    return NULLREG;
}

int asm64_for(Node *root, int cmd)
{
    int last_s, last_e, reg, enter;

    // save labels
    last_s = start;
    start = label++;

    last_e = end;
    end = label++;

    enter = label++;
    func->level++;

    // loop variable
    rfree_all();
    gen(root->left->left, NULLREG, cmd);
    rfree_all();

    asm64_jump(enter);
    asm64_label(start);

    // increment/decrement
    rfree_all();
    gen(root->right, NULLREG, cmd);
    rfree_all();

    asm64_label(enter);

    // condition
    rfree_all();
    reg = gen(root->mid, NULLREG, cmd);
    asm64_cmpz(reg);
    asm64_jumpeq(end);
    rfree_all();

    // body
    rfree_all();
    gen(root->left->right, NULLREG, cmd);
    rfree_all();

    // end
    asm64_jump(start);
    asm64_label(end);

    func->level--;
    asm64_stackfree();

    // put labels back
    start = last_s;
    end = last_e;

    return NULLREG;
}

int asm64_ulabel(char *name)
{
    fprintf(out_f, "%s:\n", name);
    return NULLREG;
}

int asm64_goto(char *name)
{
    fprintf(out_f, "\tjmp\t\t%s\n", name);
    return NULLREG;
}

int asm64_incdec(int reg, char *ins)
{

    if (type.addr)
    {
        fprintf(out_f, "\t%s\t\tQWORD [%s]\n", ins, reginfo->reglist[reg]);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\t%s\t\tBYTE [%s]\n", ins, reginfo->reglist[reg]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\t%s\t\tWORD [%s]\n", ins, reginfo->reglist[reg]);
        break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\t%s\t\tDWORD [%s]\n", ins, reginfo->reglist[reg]);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\t%s\t\tQWORD [%s]\n", ins, reginfo->reglist[reg]);
        break;
    }

    return reg;
}

int asm64_asm(char *code)
{
    Tok  tok;
    Sym *sym;
    char name[128];
    int c, len = strlen(code);

    for (int i = 0; i < len; i++)
    {
        c = code[i];

        /* ABC variable reference in inline assembly */
        if (c == '(')
        {
            i++;
            c = code[i];

            for (int j = 0; j < len && c != ')'; j++)
            {
                name[j] = c;
                name[j + 1] = 0;
                c = code[i + j + 1];
            }
            
            tok.val.i = adduniq(name);
            sym = getsym(&tok);

            switch (sym->class)
            {
            case C_GLOB:
                fprintf(out_f, "%s", sym->name);
            case C_LOCL:
                fprintf(out_f, "[sp - %d]", sym->offs);
            }

            i += strlen(name);
        }
        else
        {
            fputc(c, out_f);
        }
    }

    return NULLREG;
}

void asm64_preamble(void)
{
    // main directive
    fprintf(out_f, "\tglobal  main\n\n");

    // text section
    fprintf(out_f, "\tsection .text\n");
}

void asm64_postamble(void)
{
    // externs
    for (int i = 0; i < glob->used; i++)
    {
        Sym *sym = glob->get[i];
        if (sym->class == C_EXTN)
        {
            fprintf(out_f, "\textern %s\n", sym->name);
        }
    }

    // bss section
    fprintf(out_f, "\n\tsection .bss\n");
    for (size_t i = 0; glob && i < glob->used; i++)
    {
        Sym *sym = glob->get[i];
        if (sym->class == C_GLOB && !sym->local)
        {
            fprintf(out_f, "%s:\t\t\t\tresb %d\n", sym->name, type2size(sym->type) * sym->size);
        }
    }

    // data section
    fprintf(out_f, "\n\tsection .data\n");
    for (size_t i = 0; glob && i < glob->used; i++)
    {
        Sym *sym = glob->get[i];
        if (sym->class == C_DATA)
        {
            fprintf(out_f, "S%d:\t\t\t\tdb ", i);
            for (int j = 0; j < sym->size; j++)
            {
                fprintf(out_f, "%d, ", sym->name[j]);
            }
            fprintf(out_f, "0\n");
        }
    }
}
