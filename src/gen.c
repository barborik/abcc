#include "includes.h"

int label = 1, top = 0;

Sym *sym;
RegInfo *reginfo = &std64;

int ralloc()
{
    for (int i = 0; i < reginfo->nregs; i++)
    {
        if (!reginfo->regalloc[i])
        {
            reginfo->regalloc[i] = 1;
            return i;
        }
    }

    return -1;
}

void rfree(int reg)
{
    reginfo->regalloc[reg] = 0;
}

void rfree_all()
{
    memset(reginfo->regalloc, 0, reginfo->nregs * sizeof(int));
}

int asm_load(int val)
{
    int reg = ralloc();
    fprintf(out_f, "\tmov\t\t%s, %d\n", reginfo->reglist[reg], val);
    return reg;
}

int asm_loadstr(int id)
{
    int reg = ralloc();
    fprintf(out_f, "\tmov\t\t%s, S%d\n", reginfo->reglist[reg], id);
    return reg;
}

int asm_add(int reg0, int reg1)
{
    fprintf(out_f, "\tadd\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_sub(int reg0, int reg1)
{
    fprintf(out_f, "\tsub\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_mul(int reg0, int reg1)
{
    fprintf(out_f, "\timul\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_div(int reg0, int reg1)
{
    fprintf(out_f, "\tmov\t\trax, %s\n", reginfo->reglist[reg0]);
    fprintf(out_f, "\tcqo\n");
    fprintf(out_f, "\tidiv\t%s\n", reginfo->reglist[reg1]);
    fprintf(out_f, "\tmov\t\t%s, rax\n", reginfo->reglist[reg0]);
    rfree(reg1);
    return reg0;
}

int asm_lognot(int reg)
{
    int tmp = ralloc();
    fprintf(out_f, "\txor\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[tmp]);
    fprintf(out_f, "\ttest\t%s, %s\n", reginfo->reglist[reg], reginfo->reglist[reg]);
    fprintf(out_f, "\tsetz\t%s\n", reginfo->reglist8[tmp]);
    fprintf(out_f, "\tmov\t\t%s, %s\n", reginfo->reglist[reg], reginfo->reglist[tmp]);
    rfree(tmp);
    return reg;
}

int asm_bitnot(int reg)
{
    fprintf(out_f, "\tnot\t\t%s\n", reginfo->reglist[reg]);
    return reg;
}

int asm_or(int reg0, int reg1)
{
    fprintf(out_f, "\tor\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_addr(Sym *sym)
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

int asm_deref(int reg)
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

int asm_loadglob(Sym *sym)
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

int asm_storeglob(int reg, Sym *sym)
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

int asm_loadlocl(Sym *sym)
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

int asm_storelocl(int reg, Sym *sym)
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

int asm_storederef(int reg0, int reg1, Sym *sym)
{
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

void asm_label(int l)
{
    fprintf(out_f, "L%d:\n", l);
}

int asm_cmpset(int reg0, int reg1, char *ins)
{
    fprintf(out_f, "\tcmp\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    fprintf(out_f, "\t%s\t%s\n", ins, reginfo->reglist8[reg0]);
    fprintf(out_f, "\tand\t\t%s, 0xff\n", reginfo->reglist[reg0]);
    return reg0;
}

void asm_cmpz(int reg)
{
    fprintf(out_f, "\tcmp\t\t%s, 0\n", reginfo->reglist[reg]);
}

void asm_jumpeq(int l)
{
    fprintf(out_f, "\tje\t\tL%d\n", l);
}

void asm_jump(int l)
{
    fprintf(out_f, "\tjmp\t\tL%d\n", l);
}

int asm_alloc(Sym *sym)
{
    dl_add(func->stack, &sym);
    fprintf(out_f, "\tsub\t\trsp, %d\n", type2size(sym->type) * sym->size);
    
    top += type2size(sym->type) * sym->size;
    sym->offs = top;
    return NULLREG;
}

int asm_func(Node *root)
{
    top = 0;
    fprintf(out_f, "%s:\n", func->name);
    fprintf(out_f, "\tpush\trbp\n");
    fprintf(out_f, "\tmov\t\trbp, rsp\n");
    
    reginfo = &elf64;

    for (int i = 0; i < func->argc; i++)
    {
        Sym *sym = func->local->get[i];
        type = sym->type;

        asm_alloc(sym);
        asm_storelocl(i, sym);
    }

    reginfo = &std64;

    gen(root, NULLREG, A_EXEC);
    return NULLREG;
}

int asm_call(int reg, Node *args)
{
    RegInfo *tmp;
    int align, offs, *regalloc;

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
    reginfo = &elf64;
    rfree_all();
    reginfo = tmp;

    tmp = reginfo; 
    gen(args, NULLREG, A_ARGS);
    reginfo = tmp;

    memcpy(reginfo->regalloc, regalloc, reginfo->nregs * sizeof(int));
    free(regalloc);

    /*for (int i = 0; i < func->stack->used; i++)
    {
        Sym *sym = *(Sym**)func->stack->get[i];
        align += type2size(sym->type) * sym->size;
    }*/
    //align = top % 16;
    align = top % 16;

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

int asm_ret(int reg)
{
    if (reg != NULLREG)
    {
        fprintf(out_f, "\tmov\t\trax, %s\n", reginfo->reglist[reg]);
    }
    fprintf(out_f, "\tleave\n");
    fprintf(out_f, "\tret\n\n");
    return NULLREG;
}

void asm_stackfree()
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

int asm_if(Node *root, int cmd)
{
    int end, reg;

    func->level++;

    reg = gen(root->mid, NULLREG, cmd);
    asm_cmpz(reg);
    end = label++;
    asm_jumpeq(end);
    rfree_all();
    if (root->left)
    {
        gen(root->left, NULLREG, cmd);
    }
    asm_label(end);
    
    func->level--;
    asm_stackfree();

    return NULLREG;
}

int asm_while(Node *root, int cmd)
{
    int start, end, reg;

    func->level++;

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

    func->level--;
    asm_stackfree();

    return NULLREG;
}

int asm_incdec(int reg, char *ins)
{
    if (type.addr)
    {
        fprintf(out_f, "\t%s\t\tQWORD [%s]\n", ins, reginfo->reglist64[reg]);

        asm_deref(reg);
        return reg;
    }

    switch (type.type)
    {
    case LT_U8:
    case LT_I8:
        fprintf(out_f, "\t%s\t\tBYTE [%s]\n", ins, reginfo->reglist8[reg]);
        break;
    case LT_U16:
    case LT_I16:
        fprintf(out_f, "\t%s\t\tWORD [%s]\n", ins, reginfo->reglist16[reg]);
        break;
    case LT_U32:
    case LT_I32:
        fprintf(out_f, "\t%s\t\tDWORD [%s]\n", ins, reginfo->reglist32[reg]);
        break;
    case LT_U64:
    case LT_I64:
        fprintf(out_f, "\t%s\t\tQWORD [%s]\n", ins, reginfo->reglist64[reg]);
        break;
    }

    asm_deref(reg);
    return reg;
}

void asm_preamble()
{
    // main directive
    fprintf(out_f, "\tglobal  main\n\n");

    // text section
    fprintf(out_f, "\tsection .text\n");
}

void asm_postamble()
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

int gen(Node *root, int reg, int cmd)
{
    int id;
    Tok *tok;
    int leftreg = NULLREG, rightreg = NULLREG;

    if (!root)
    {
        return NULLREG;
    }

    tok = &root->token;

    if (tok->token != ST_JOIN && !isblock(tok) && cmd != A_WALK)
    {
        if (root->left && !isblock(tok->token))
        {
            leftreg = gen(root->left, NULLREG, cmd);
        }
        if (root->right && !isblock(tok->token))
        {
            rightreg = gen(root->right, leftreg, cmd);
        }
    }

    switch (cmd)
    {
    case A_EXEC: reginfo = &std64; break;
    case A_ARGS: reginfo = &elf64; break;
    }

    // DEBUG
    fprintf(out_f, "\t; line %d\n", tok->line);

    asm_stackfree();

    switch (tok->token)
    {
    case ST_ADD: return asm_add(leftreg, rightreg);
    case ST_SUB: return asm_sub(leftreg, rightreg);
    case ST_MUL: return asm_mul(leftreg, rightreg);
    case ST_DIV: return asm_div(leftreg, rightreg);
    case ST_INTLIT:
        type.type = LT_I32;
        type.addr = 0;
        return asm_load(tok->val.i);
    case ST_CHARLIT: 
        type.type = LT_I8;
        type.addr = 0;
        return asm_load(tok->val.c);
    case ST_STRLIT:
        id = globid(tok);
        type.type = LT_I8;
        type.addr = 1;
        return asm_loadstr(id);
    case ST_IDENT:
        sym = getsym(tok);
        type = sym->type;
        if (getrval(root))
        {
            switch (sym->class)
            {
            case C_LOCL: return asm_loadlocl(sym);
            case C_GLOB:
            case C_EXTN:
            case C_DATA:
                return asm_loadglob(sym);
            }
        }
        return NULLREG;
    case ST_ASSIGN:
        tok = &root->right->token;
        sym = getsym(tok);

        switch (tok->token)
        {
        case ST_IDENT:
            switch (sym->class)
            {
            case C_LOCL: return asm_storelocl(leftreg, sym);
            case C_GLOB: return asm_storeglob(leftreg, sym);
            }
            break;
        case ST_DEREF: return asm_storederef(leftreg, rightreg, sym);
        }
        break;
    case ST_DEREF:
        if (getrval(root))
        {
            return asm_deref(leftreg);
        }
        return leftreg;
    case ST_ADDR: return asm_addr(sym);
    case ST_ALLOC:
        sym = getsym(tok); 
        return asm_alloc(sym);
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
    case ST_EQ: return asm_cmpset(leftreg, rightreg, "sete");
    case ST_NE: return asm_cmpset(leftreg, rightreg, "setne");
    case ST_GT: return asm_cmpset(leftreg, rightreg, "setg");
    case ST_LT: return asm_cmpset(leftreg, rightreg, "setl");
    case ST_GE: return asm_cmpset(leftreg, rightreg, "setge");
    case ST_LE: return asm_cmpset(leftreg, rightreg, "setle");
    case ST_LOGNOT: return asm_lognot(leftreg);
    case ST_BITNOT: return asm_bitnot(leftreg);
    case ST_LOGOR:
    case ST_BITOR: return asm_or(leftreg, rightreg);
    case ST_IF: return asm_if(root, cmd);
    case ST_WHILE: return asm_while(root, cmd);
    case ST_FUNC: return asm_func(root->left);
    case ST_CALL:
        //ncall++;
        leftreg = asm_call(leftreg, root->mid);
        //ncall--;
        return leftreg;
    case ST_RETURN: return asm_ret(leftreg);
    case ST_INC: return asm_incdec(leftreg, "inc");
    case ST_DEC: return asm_incdec(leftreg, "dec");
    }

    printf("ERROR: line %d, token: %d\n", tok->line, tok->token);
    exit(1);
}