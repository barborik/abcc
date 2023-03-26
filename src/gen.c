#include "includes.h"

reginfo_t *reginfo = &std64;

FILE *asmf;
int label = 1;
sym_t *sym;  // last symbol
sym_t *func; // current function

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
    fprintf(asmf, "\tmov\t\t%s, %d\n", reginfo->reglist[reg], val);
    return reg;
}

int asm_loadstr(int id)
{
    int reg = ralloc();
    fprintf(asmf, "\tmov\t\t%s, S%d\n", reginfo->reglist[reg], id);
    return reg;
}

int asm_add(int reg0, int reg1)
{
    fprintf(asmf, "\tadd\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_sub(int reg0, int reg1)
{
    fprintf(asmf, "\tsub\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_mul(int reg0, int reg1)
{
    fprintf(asmf, "\timul\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    rfree(reg1);
    return reg0;
}

int asm_div(int reg0, int reg1)
{
    fprintf(asmf, "\tmov\t\trax, %s\n", reginfo->reglist[reg0]);
    fprintf(asmf, "\tcqo\n");
    fprintf(asmf, "\tidiv\t%s\n", reginfo->reglist[reg1]);
    fprintf(asmf, "\tmov\t\t%s, rax\n", reginfo->reglist[reg0]);
    rfree(reg1);
    return reg0;
}

int asm_lognot(int reg)
{
    int tmp = ralloc();
    fprintf(asmf, "\txor\t\t%s, %s\n", reginfo->reglist[tmp], reginfo->reglist[tmp]);
    fprintf(asmf, "\ttest\t%s, %s\n", reginfo->reglist[reg], reginfo->reglist[reg]);
    fprintf(asmf, "\tsetz\t%s\n", reginfo->reglist8[tmp]);
    fprintf(asmf, "\tmov\t\t%s, %s\n", reginfo->reglist[reg], reginfo->reglist[tmp]);
    rfree(tmp);
    return reg;
}

int asm_bitnot(int reg)
{
    fprintf(asmf, "\tnot\t\t%s\n", reginfo->reglist[reg]);
    return reg;
}

int asm_addr(token_t *t)
{
    sym_t *sym;
    int reg = ralloc();

    if (t->class == C_LOCL)
    {
        sym = func->local->get[t->val.id];
        fprintf(asmf, "\tmov\t\t%s, rbp\n", reginfo->reglist[reg]);
        fprintf(asmf, "\tsub\t\t%s, %d\n", reginfo->reglist[reg], sym->offs);
    }
    else
    {
        sym = glob->get[t->val.id];
        fprintf(asmf, "\tmov\t\t%s, %s\n", reginfo->reglist[reg], sym->name);
    }

    return reg;
}

int asm_deref(int reg, int type)
{
    switch (type)
    {
    case T_U8:
    case T_I8:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reginfo->reglist8[reg], reginfo->reglist[reg]);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reginfo->reglist16[reg], reginfo->reglist[reg]);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reginfo->reglist32[reg], reginfo->reglist[reg]);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reginfo->reglist64[reg], reginfo->reglist[reg]);
        break;
    default:
        fprintf(asmf, "\tmov\t\t%s, [%s]\n", reginfo->reglist64[reg], reginfo->reglist[reg]);
        break;
    }

    return reg;
}

int asm_loadglob(sym_t *sym)
{
    int reg = ralloc();

    switch (sym->type)
    {
    // value types
    case T_U8:  fprintf(asmf, "\tmovzx\t%s, BYTE [%s]\n", reginfo->reglist32[reg], sym->name); break;
    case T_I8:  fprintf(asmf, "\tmovsx\t%s, BYTE [%s]\n", reginfo->reglist32[reg], sym->name); break;
    case T_U16: fprintf(asmf, "\tmovzx\t%s, WORD [%s]\n", reginfo->reglist32[reg], sym->name); break;
    case T_I16: fprintf(asmf, "\tmovsx\t%s, WORD [%s]\n", reginfo->reglist32[reg], sym->name); break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t%s, DWORD [%s]\n", reginfo->reglist32[reg], sym->name);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t%s, QWORD [%s]\n", reginfo->reglist64[reg], sym->name);
        break;

    // pointers
    default:
        fprintf(asmf, "\tmov\t\t%s, QWORD [%s]\n", reginfo->reglist64[reg], sym->name);
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
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist8[reg]);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist16[reg]);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist32[reg]);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist64[reg]);
        break;
    default:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", sym->name, reginfo->reglist64[reg]);
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
    case T_U8:  fprintf(asmf, "\tmovzx\t%s, BYTE [rbp - %d]\n", reginfo->reglist32[reg], sym->offs); break;
    case T_I8:  fprintf(asmf, "\tmovsx\t%s, BYTE [rbp - %d]\n", reginfo->reglist32[reg], sym->offs); break;
    case T_U16: fprintf(asmf, "\tmovzx\t%s, WORD [rbp - %d]\n", reginfo->reglist32[reg], sym->offs); break;
    case T_I16: fprintf(asmf, "\tmovsx\t%s, WORD [rbp - %d]\n", reginfo->reglist32[reg], sym->offs); break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t%s, DWORD [rbp - %d]\n", reginfo->reglist32[reg], sym->offs);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t%s, QWORD [rbp - %d]\n", reginfo->reglist64[reg], sym->offs);
        break;

    // pointers
    default:
        fprintf(asmf, "\tmov\t\t%s, QWORD [rbp - %d]\n", reginfo->reglist64[reg], sym->offs);
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
        fprintf(asmf, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist8[reg]);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist16[reg]);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist32[reg]);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist64[reg]);
        break;
    default:
        fprintf(asmf, "\tmov\t\t[rbp - %d], %s\n", sym->offs, reginfo->reglist64[reg]);
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
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist8[reg0]);
        break;
    case T_U16:
    case T_I16:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist16[reg0]);
        break;
    case T_U32:
    case T_I32:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist32[reg0]);
        break;
    case T_U64:
    case T_I64:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist64[reg0]);
        break;
    default:
        fprintf(asmf, "\tmov\t\t[%s], %s\n", reginfo->reglist[reg1], reginfo->reglist64[reg0]);
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
    fprintf(asmf, "\tcmp\t\t%s, %s\n", reginfo->reglist[reg0], reginfo->reglist[reg1]);
    fprintf(asmf, "\t%s\t%s\n", ins, reginfo->reglist8[reg0]);
    fprintf(asmf, "\tand\t\t%s, 0xff\n", reginfo->reglist[reg0]);
    return reg0;
}

void asm_cmpz(int reg)
{
    fprintf(asmf, "\tcmp\t\t%s, 0\n", reginfo->reglist[reg]);
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
    fprintf(asmf, "\tpush\trbp\n");
    fprintf(asmf, "\tmov\t\trbp, rsp\n");
    
    reginfo = &elf64;

    for (int i = 0; i < func->argc; i++)
    {
        sym_t *sym = func->local->get[i];

        sym->level = 1;
        dl_add(func->stack, &sym);

        for (int j = 0; j < func->stack->used; j++)
        {
            sym_t *tmp = *(sym_t **)func->stack->get[j];
            sym->offs += type2size(tmp->type);
        }

        fprintf(asmf, "\tsub\t\trsp, %d\n", type2size(sym->type));
        asm_storelocl(i, sym);
    }

    reginfo = &std64;

    gen(root, NULLREG, A_EXEC);
    return NULLREG;
}

int asm_alloc(sym_t *sym)
{
    dl_add(func->stack, &sym);
    fprintf(asmf, "\tsub\t\trsp, %d\n", type2size(sym->type) * sym->size);
    for (int i = 0; i < func->stack->used; i++)
    {
        sym_t *tmp = *(sym_t **)func->stack->get[i];
        sym->offs += type2size(tmp->type) * tmp->size;
    }
    return NULLREG;
}

int asm_call(asnode_t *args, sym_t *sym)
{
    reginfo_t *tmp;
    int reg = NULLREG, align = 0, *regalloc;

    // push used registers
    for (int i = 0; i < reginfo->nregs; i++)
    {
        if (reginfo->regalloc[i])
        {
            fprintf(asmf, "\tpush\t%s\n", reginfo->reglist[i]);
            align += 8;
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

    for (int i = 0; i < func->stack->used; i++)
    {
        sym_t *sym = func->stack->get[i];
        align += type2size(sym->type) * sym->size;
    }
    align = align % 16;

    if (sym->argc < 0)
    {
        fprintf(asmf, "\txor\t\trax, rax\n");
    }

    fprintf(asmf, "\tsub\t\trsp, %d\n", align);
    fprintf(asmf, "\tcall\t%s\n", sym->name);
    fprintf(asmf, "\tadd\t\trsp, %d\n", align);

    if (sym->type != T_U0 && sym->type != T_I0)
    {
        reg = ralloc();
        fprintf(asmf, "\tmov\t\t%s, rax\n", reginfo->reglist[reg]);
    }

    // pop used registers back
    for (int i = reginfo->nregs - 1; i >= 0; i--)
    {
        if (reginfo->regalloc[i] && i != reg)
        {
            fprintf(asmf, "\tpop\t\t%s\n", reginfo->reglist[i]);
        }
    }
    
    return reg;
}

int asm_ret(int reg)
{
    if (reg != NULLREG)
    {
        fprintf(asmf, "\tmov\t\trax, %s\n", reginfo->reglist[reg]);
    }
    fprintf(asmf, "\tleave\n");
    fprintf(asmf, "\tret\n\n");
    return NULLREG;
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

int asm_if(asnode_t *root, int cmd)
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

int asm_while(asnode_t *root, int cmd)
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
    fprintf(asmf, "\n\tsection .bss\n");
    for (size_t i = 0; glob && i < glob->used; i++)
    {
        sym_t *sym = glob->get[i];
        if (sym->class == C_GLOB)
        {
            fprintf(asmf, "%s:\t\t\t\tresb %d\n", sym->name, type2size(sym->type) * sym->size);
        }
    }

    // data section
    fprintf(asmf, "\n\tsection .data\n");
    for (size_t i = 0; glob && i < glob->used; i++)
    {
        sym_t *sym = glob->get[i];
        if (sym->class == C_DATA)
        {
            fprintf(asmf, "S%d:\t\t\t\tdb ", i);
            for (int j = 0; j < sym->size; j++)
            {
                fprintf(asmf, "%d, ", sym->name[j]);
            }
            fprintf(asmf, "0\n");
        }
    }
}

int gen(asnode_t *root, int reg, int cmd)
{
    token_t *t;
    int leftreg = NULLREG, rightreg = NULLREG;

    if (!root)
    {
        return NULLREG;
    }

    t = root->token;

    if (t->token == ST_CALL)
    {
        cmd = A_WALK;
    }

    if (t->token != ST_JOIN && !isblock(t) && cmd != A_WALK)
    {
        if (root->left && !isblock(t->token))
        {
            leftreg = gen(root->left, NULLREG, cmd);
        }
        if (root->right && !isblock(t->token))
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
    fprintf(asmf, "\t; line %d\n", t->line);

    asm_stackfree();

    switch (t->token)
    {
    case ST_ADD: return asm_add(leftreg, rightreg);
    case ST_SUB: return asm_sub(leftreg, rightreg);
    case ST_MUL: return asm_mul(leftreg, rightreg);
    case ST_DIV: return asm_div(leftreg, rightreg);
    case ST_INTLIT: return asm_load(t->val.i);
    case ST_CHARLIT: return asm_load(t->val.c);
    case ST_STRLIT: return asm_loadstr(t->val.id);
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