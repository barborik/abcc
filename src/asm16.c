#include "includes.h"

void asm16_stackfree(void)
{
    int add = 0;
    for (int i = 0; i < func->stack->used; i++)
    {
        Sym *sym = *(Sym **)func->stack->get[i];
        if (sym->level > func->level)
        {
            add += type2size(sym->type) * sym->type.size;
            dl_rem(func->stack, i);
            i--;
        }
    }
    if (add)
    {
        top -= add;
        fprintf(out_f, "\tadd\t\tsp, %d\n", add);
    }
}

int asm16_load(int val)
{
    int reg = ralloc();
    fprintf(out_f, "\tmov\t\t%s, %d\n", reginfo->reglist[reg], val);
    return reg;
}

int asm16_loadstr(int id)
{
    int reg = ralloc();
    fprintf(out_f, "\tmov\t\t%s, S%d\n", reginfo->reglist[reg], id);
    return reg;
}

int asm16_add(int reg0, int reg1)
{
    fprintf(out_f, "\tadd\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm16_sub(int reg0, int reg1)
{
    fprintf(out_f, "\tsub\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm16_mul(int reg0, int reg1)
{
    if (type.addr)
    {
        fprintf(out_f, "\tpush\tax\n");
        fprintf(out_f, "\tmov\t\tax, %s\n", reginfo->reglist[reg0]);
        fprintf(out_f, "\tmul\t\t%s\n", reginfo->reglist[reg1]);
        fprintf(out_f, "\tmov\t\t%s, ax\n", reginfo->reglist[reg0]);
        fprintf(out_f, "\tpop\t\tax\n");
        rfree(reg1);
        return reg0;
    }

    switch (type.type)
    {
    case LT_I8:
    case LT_I16:
        fprintf(out_f, "\timul\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
        break;
    case LT_U8:
    case LT_U16:
        fprintf(out_f, "\tpush\tax\n");
        fprintf(out_f, "\tmov\t\tax, %s\n", reginfo->reglist[reg0]);
        fprintf(out_f, "\tmul\t\t%s\n", reginfo->reglist[reg1]);
        fprintf(out_f, "\tmov\t\t%s, ax\n", reginfo->reglist[reg0]);
        fprintf(out_f, "\tpop\t\tax\n");
        break;
    }

    rfree(reg1);
    return reg0;
}

int asm16_div(int reg0, int reg1)
{
    fprintf(out_f, "\tpush\tax\n");
    fprintf(out_f, "\tpush\tbx\n");
    fprintf(out_f, "\tmov\t\tax, %s\n", reginfo->reglist[reg0]);
    fprintf(out_f, "\tmov\t\tbx, %s\n", reginfo->reglist[reg1]);
    fprintf(out_f, "\tcwd\n");

    if (type.addr)
    {
        fprintf(out_f, "\tdiv\t\tbx\n");

        fprintf(out_f, "\tmov\t\t%s, ax\n", reginfo->reglist[reg0]);
        fprintf(out_f, "\tpop\t\tbx\n");
        fprintf(out_f, "\tpop\t\tax\n");

        rfree(reg1);
        return reg0;
    }

    switch (type.type)
    {
    case LT_I8:
    case LT_I16:
        fprintf(out_f, "\tidiv\tbx\n");
        break;
    case LT_U8:
    case LT_U16:
        fprintf(out_f, "\tdiv\t\tbx\n");
        break;
    }

    fprintf(out_f, "\tmov\t\t%s, ax\n", reginfo->reglist[reg0]);

    if (reg0 == 0 || reg0 == 1)
    {
        fprintf(out_f, "\tadd\t\tsp, 2\n");
        fprintf(out_f, "\tpop\t\tax\n");
    }
    else
    {
        fprintf(out_f, "\tpop\t\tbx\n");
        fprintf(out_f, "\tpop\t\tax\n");
    }

    rfree(reg1);
    return reg0;
}

int asm16_mod(int reg0, int reg1)
{
    asm16_div(reg0, reg1);
    fprintf(out_f, "\tmov\t\t%s, dx\n", reginfo->reglist[reg0]);
    return reg0;
}

int asm16_lognot(int reg)
{
    int tmp = ralloc();
    fprintf(out_f, "\txor\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[tmp]);
    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg], reginfo->reglist[reg]);
    fprintf(out_f, "\tsetz\t%s\n", reginfo->reglist8[tmp]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg], reginfo->reglist[tmp]);
    rfree(tmp);
    return reg;
}

int asm16_bitnot(int reg)
{
    if (type.addr)
    {
        fprintf(out_f, "\tnot\t\t%s\n", reginfo->reglist16[reg]);
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
    }

    return reg;
}

int asm16_neg(int reg)
{
    if (type.addr)
    {
        fprintf(out_f, "\tneg\t\t%s\n", reginfo->reglist16[reg]);
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
    }

    return reg;
}

int asm16_bitor(int reg0, int reg1)
{
    fprintf(out_f, "\tor\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm16_bitand(int reg0, int reg1)
{
    fprintf(out_f, "\tand\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm16_logor(int reg0, int reg1)
{
    int tmp = ralloc();
    fprintf(out_f, "\txor\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[tmp]);

    /* REG 0 */
    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg0]);

    fprintf(out_f, "\tpush\t%s\n", reginfo->reglist[0]);
    fprintf(out_f, "\tsetne\t%s\n", reginfo->reglist8[0]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[0]);
    fprintf(out_f, "\tpop\t\t%s\n", reginfo->reglist[0]);

    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[tmp]);

    /* REG 1 */
    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg1], reginfo->reglist[reg1]);

    fprintf(out_f, "\tpush\t%s\n", reginfo->reglist[0]);
    fprintf(out_f, "\tsetne\t%s\n", reginfo->reglist8[0]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[0]);
    fprintf(out_f, "\tpop\t\t%s\n", reginfo->reglist[0]);

    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg1], reginfo->reglist[tmp]);

    rfree(tmp);
    return asm16_bitor(reg0, reg1);
}

int asm16_logand(int reg0, int reg1)
{
    int tmp = ralloc();
    fprintf(out_f, "\txor\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[tmp]);

    /* REG 0 */
    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg0]);

    fprintf(out_f, "\tpush\t%s\n", reginfo->reglist[0]);
    fprintf(out_f, "\tsetne\t%s\n", reginfo->reglist8[0]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[0]);
    fprintf(out_f, "\tpop\t\t%s\n", reginfo->reglist[0]);

    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[tmp]);

    /* REG 1 */
    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg1], reginfo->reglist[reg1]);

    fprintf(out_f, "\tpush\t%s\n", reginfo->reglist[0]);
    fprintf(out_f, "\tsetne\t%s\n", reginfo->reglist8[0]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[0]);
    fprintf(out_f, "\tpop\t\t%s\n", reginfo->reglist[0]);

    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg1], reginfo->reglist[tmp]);

    rfree(tmp);

    return asm16_bitand(reg0, reg1);
}

int asm16_bitxor(int reg0, int reg1)
{
    fprintf(out_f, "\txor\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm16_lshift(int reg0, int reg1)
{
    fprintf(out_f, "\tpush\tcx\n");
    fprintf(out_f, "\tpush\tax\n");

    fprintf(out_f, "\tmov\t\tax, %s\n", reginfo->reglist[reg0]);
    fprintf(out_f, "\tmov\t\tcx, %s\n", reginfo->reglist[reg1]);
    fprintf(out_f, "\tshl\t\tax, cl\n");
    fprintf(out_f, "\tmov\t\t%s, ax\n", reginfo->reglist[reg0]);

    fprintf(out_f, "\tpop\t\tax\n");
    fprintf(out_f, "\tpop\t\tcx\n");

    rfree(reg1);
    return reg0;
}

int asm16_rshift(int reg0, int reg1)
{
    fprintf(out_f, "\tpush\tcx\n");
    fprintf(out_f, "\tpush\tax\n");

    fprintf(out_f, "\tmov\t\tax, %s\n", reginfo->reglist[reg0]);
    fprintf(out_f, "\tmov\t\tcx, %s\n", reginfo->reglist[reg1]);
    fprintf(out_f, "\tshr\t\tax, cl\n");
    fprintf(out_f, "\tmov\t\t%s, ax\n", reginfo->reglist[reg0]);

    fprintf(out_f, "\tpop\t\tax\n");
    fprintf(out_f, "\tpop\t\tcx\n");

    rfree(reg1);
    return reg0;
}

int asm16_addr(Sym *sym)
{
    int reg = ralloc();

    if (sym->class == C_LOCL)
    {
        fprintf(out_f, "\tmov\t\t%s, bp\n", reginfo->reglist[reg]);
        fprintf(out_f, "\tsub\t\t%s, %d\n", reginfo->reglist[reg], sym->offs);
    }
    else
    {
        fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg], sym->name);
    }

    type.addr++;
    return reg;
}

int asm16_deref(int reg)
{
    type.addr--;

    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t%s, [%s]\n", reginfo->reglist16[reg], reginfo->reglist[reg]);
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
    }

    return reg;
}

int asm16_loadglob(Sym *sym)
{
    int reg = ralloc();

    if (type.addr || sym->local)
    {
        fprintf(out_f, "\tmov\t\t%s, QWORD %s\n", reginfo->reglist16[reg], sym->name);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
        fprintf(out_f, "\tmovzx\t%s, BYTE [%s]\n", reginfo->reglist16[reg], sym->name);
        break;
    case LT_I8:
        fprintf(out_f, "\tmovsx\t%s, BYTE [%s]\n", reginfo->reglist16[reg], sym->name);
        break;
    case LT_U16:
        fprintf(out_f, "\tmov\t\t%s, WORD [%s]\n", reginfo->reglist16[reg], sym->name);
        break;
    case LT_I16:
        fprintf(out_f, "\tmov\t\t%s, WORD [%s]\n", reginfo->reglist16[reg], sym->name);
        break;
    }

    return reg;
}

int asm16_storeglob(int reg, Sym *sym)
{
    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist16[reg]);
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
    }

    return reg;
}

int asm16_loadlocl(Sym *sym)
{
    int reg = ralloc();

    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t%s, QWORD [bp - %d]\n", reginfo->reglist16[reg], sym->offs);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
        fprintf(out_f, "\tmovzx\t%s, BYTE [bp - %d]\n", reginfo->reglist16[reg], sym->offs);
        break;
    case LT_I8:
        fprintf(out_f, "\tmovsx\t%s, BYTE [bp - %d]\n", reginfo->reglist16[reg], sym->offs);
        break;
    case LT_U16:
        fprintf(out_f, "\tmov\t\t%s, WORD [bp - %d]\n", reginfo->reglist16[reg], sym->offs);
        break;
    case LT_I16:
        fprintf(out_f, "\tmov\t\t%s, WORD [bp - %d]\n", reginfo->reglist16[reg], sym->offs);
        break;
    }

    return reg;
}

int asm16_storelocl(int reg, Sym *sym)
{
    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t[bp - %d], %s\n", sym->offs, reginfo->reglist16[reg]);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\tmov\t\t[bp - %d], %s\n", sym->offs, reginfo->reglist8[reg]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\tmov\t\t[bp - %d], %s\n", sym->offs, reginfo->reglist16[reg]);
        break;
    }

    return reg;
}

int asm16_storederef(int reg0, int reg1, Sym *sym)
{
    if (type.addr)
        type.addr--;

    if (type.addr)
    {
        fprintf(out_f, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist16[reg0]);
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
    }
    return reg0;
}

int asm16_push(int reg)
{
    if (type.addr)
    {
        pushed += 2;
        fprintf(out_f, "\tpush\t%s\n", reginfo->reglist[reg]);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        pushed += 2;
        fprintf(out_f, "\tpush\t%s\n", reginfo->reglist16[reg]);
        break;
    case LT_U16:
    case LT_I16:
        pushed += 2;
        fprintf(out_f, "\tpush\t%s\n", reginfo->reglist16[reg]);
        break;
    }

    return reg;
}

void asm16_label(int l)
{
    fprintf(out_f, "L%d:\n", l);
}

int asm16_cmpset(int reg0, int reg1, char *ins)
{
    if (type.addr)
    {
        fprintf(out_f, "\tcmp\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
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
    }

    fprintf(out_f, "\t%s\t%s\n", ins, reginfo->reglist8[reg0]);
    fprintf(out_f, "\tand\t\t%s, 0xff\n", reginfo->reglist[reg0]);
    return reg0;
}

void asm16_cmpz(int reg)
{
    fprintf(out_f, "\tcmp\t\t%s, 0\n", reginfo->reglist[reg]);
}

void asm16_jumpeq(int l)
{
    fprintf(out_f, "\tje\t\tL%d\n", l);
}

void asm16_jump(int l)
{
    fprintf(out_f, "\tjmp\t\tL%d\n", l);
}

int asm16_alloc(Sym *sym)
{
    dl_add(func->stack, &sym);
    fprintf(out_f, "\tsub\t\tsp, %d\n", type2size(sym->type) * sym->type.size);

    top += type2size(sym->type) * sym->type.size;
    sym->offs = top;
    return NULLREG;
}

int asm16_func(Node *root)
{
    int offs = 2;

    top = 0;
    fprintf(out_f, "%s:\n", func->name);
    fprintf(out_f, "\tpush\tbp\n");
    fprintf(out_f, "\tmov\t\tbp, sp\n");

    for (int i = 0; i < func->argc; i++)
    {
        Sym *sym = func->local->get[i];
        type = sym->type;

        dl_add(func->stack, &sym);

        // asm16_alloc(sym);

        if (type.addr)
        {
            offs += 2;
            // fprintf(out_f, "\tmov\t\t%s, WORD [bp - %d]\n", reginfo->reglist[0], offs);
            // asm16_storelocl(0, sym);
            sym->offs = -offs;
            continue;
        }

        switch (type.type)
        {
        case LT_U8:
        case LT_I8:
            offs += 2;
            sym->offs = -offs;
            // fprintf(out_f, "\tmov\t\t%s, BYTE [bp - %d]\n", reginfo->reglist8[0], offs);
            break;
        case LT_U16:
        case LT_I16:
            offs += 2;
            sym->offs = -offs;
            // fprintf(out_f, "\tmov\t\t%s, WORD [bp - %d]\n", reginfo->reglist16[0], offs);
            break;
        }

        // asm16_storelocl(0, sym);
    }

    gen(root, NULLREG, A_EXEC);
    return NULLREG;
}

int asm16_call(int reg, Node *args)
{
    gen(args, NULLREG, A_ARGS);
    fprintf(out_f, "\tcall\t%s\n", reginfo->reglist[reg]);
    fprintf(out_f, "\tadd\t\tsp, %d\n", pushed);
    fprintf(out_f, "\tmov\t\t%s, ax\n", reginfo->reglist[reg]);
    pushed = 0;
    return reg;
}

int asm16_ret(int reg)
{
    if (reg != NULLREG)
    {
        fprintf(out_f, "\tmov\t\tax, %s\n", reginfo->reglist[reg]);
    }
    fprintf(out_f, "\tleave\n");
    fprintf(out_f, "\tret\n\n");
    return NULLREG;
}

int asm16_break(void)
{
    asm16_jump(end);
    return NULLREG;
}

int asm16_continue(void)
{
    asm16_jump(start);
    return NULLREG;
}

int asm16_if(Node *root, int cmd)
{
    int if_e, else_e, reg;

    if_e = label++;
    if (root->right)
        else_e = label++;

    func->level++;

    // condition
    reg = gen(root->mid, NULLREG, cmd);
    asm16_cmpz(reg);
    asm16_jumpeq(if_e);

    // if body
    rfree_all();
    gen(root->left, NULLREG, cmd);
    rfree_all();
    if (root->right)
        asm16_jump(else_e);
    asm16_label(if_e);

    // else body
    if (root->right)
    {
        rfree_all();
        gen(root->right, NULLREG, cmd);
        rfree_all();
        asm16_label(else_e);
    }

    func->level--;
    asm16_stackfree();

    return NULLREG;
}

int asm16_while(Node *root, int cmd)
{
    int last_s, last_e, reg;

    // save labels
    last_s = start;
    start = label++;

    last_e = end;
    end = label++;

    func->level++;

    // condition
    asm16_label(start);
    reg = gen(root->mid, NULLREG, cmd);
    asm16_cmpz(reg);

    asm16_jumpeq(end);

    // body
    rfree_all();
    gen(root->left, NULLREG, cmd);
    rfree_all();

    asm16_jump(start);
    asm16_label(end);

    func->level--;
    asm16_stackfree();

    // put labels back
    start = last_s;
    end = last_e;

    return NULLREG;
}

int asm16_for(Node *root, int cmd)
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

    asm16_jump(enter);
    asm16_label(start);

    // increment/decrement
    rfree_all();
    gen(root->right, NULLREG, cmd);
    rfree_all();

    asm16_label(enter);

    // condition
    rfree_all();
    reg = gen(root->mid, NULLREG, cmd);
    asm16_cmpz(reg);
    asm16_jumpeq(end);
    rfree_all();

    // body
    rfree_all();
    gen(root->left->right, NULLREG, cmd);
    rfree_all();

    // end
    asm16_jump(start);
    asm16_label(end);

    func->level--;
    asm16_stackfree();

    // put labels back
    start = last_s;
    end = last_e;

    return NULLREG;
}

int asm16_ulabel(char *name)
{
    fprintf(out_f, "%s:\n", name);
    return NULLREG;
}

int asm16_goto(char *name)
{
    fprintf(out_f, "\tjmp\t\t%s\n", name);
    return NULLREG;
}

int asm16_incdec(int reg, char *ins)
{
    if (type.addr)
    {
        fprintf(out_f, "\t%s\t\tDWORD [%s]\n", ins, reginfo->reglist[reg]);
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
    }

    return reg;
}

int asm16_asm(char *code)
{
    Tok tok;
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
                fprintf(out_f, "[bp - %d]", sym->offs);
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

void asm16_preamble(void)
{
    struct TPPKeyword *kword = TPPLexer_LookupKeyword("__MODE16_ORG_", strlen("__MODE16_ORG_"), 0);
    fprintf(out_f, "\t[BITS 16]\n");
    if (kword)
    {
        fprintf(out_f, "\t[ORG %s]\n\n", kword->k_macro->f_begin);
    }
    else
    {
        fprintf(out_f, "\n");
    }

    fprintf(out_f, "\tglobal  main\n\n");
    fprintf(out_f, "\tsection .text\n");
}

void asm16_postamble(void)
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
            fprintf(out_f, "%s:\t\t\t\tresb %d\n", sym->name, type2size(sym->type) * sym->type.size);
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
            for (int j = 0; j < sym->type.size; j++)
            {
                fprintf(out_f, "%d, ", sym->name[j]);
            }
            fprintf(out_f, "0\n");
        }
    }
}
