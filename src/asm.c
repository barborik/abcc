#include "includes.h"

int start, end;
int label = 1, top = 0, pushed = 0;

/* UTILITY */
int ralloc(void)
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

void rfree_all(void)
{
    memset(reginfo->regalloc, 0, reginfo->nregs * sizeof(int));
}

void (*asm_stackfree)(void);

/* LOADING */
int (*asm_load)(int val);
int (*asm_loadstr)(int id);
int (*asm_loadglob)(Sym *sym);
int (*asm_loadlocl)(Sym *sym);

/* STORING */
int (*asm_storeglob)(int reg, Sym *sym);
int (*asm_storelocl)(int reg, Sym *sym);
int (*asm_storederef)(int reg0, int reg1, Sym *sym);
int (*asm_push)(int reg);

/* ARITHMETIC OPERATIONS */
int (*asm_add)(int reg0, int reg1);
int (*asm_sub)(int reg0, int reg1);
int (*asm_mul)(int reg0, int reg1);
int (*asm_div)(int reg0, int reg1);
int (*asm_mod)(int reg0, int reg1);

/* OTHER OPERATIONS */
int (*asm_neg)(int reg);
int (*asm_incdec)(int reg, char *ins);

/* MEMORY OPERATIONS */
int (*asm_addr)(Sym *sym);
int (*asm_deref)(int reg);

/* LOGICAL OPERATIONS */
int (*asm_lognot)(int reg);
int (*asm_logor)(int reg0, int reg1);
int (*asm_logand)(int reg0, int reg1);

/* BITWISE OPERATIONS */
int (*asm_bitnot)(int reg);
int (*asm_bitor)(int reg0, int reg1);
int (*asm_bitxor)(int reg0, int reg1);
int (*asm_bitand)(int reg0, int reg1);
int (*asm_lshift)(int reg0, int reg1);
int (*asm_rshift)(int reg0, int reg1);

/* COMPLEX CONSTRUCTS */
int (*asm_if)(Node *root, int cmd);
int (*asm_while)(Node *root, int cmd);
int (*asm_for)(Node *root, int cmd);

/* FLOW CONTROL */
int (*asm_break)(void);
int (*asm_continue)(void);
int (*asm_ret)(int reg);
int (*asm_goto)(char *name);

/* OTHER */
void (*asm_label)(int l);
int (*asm_ulabel)(char *name);
int (*asm_cmpset)(int reg0, int reg1, char *ins);
void (*asm_cmpz)(int reg);
void (*asm_jumpeq)(int l);
void (*asm_jump)(int l);
int (*asm_alloc)(Sym *sym);

/* OTHER: 2 ELECTRIC BOOGALOO */
int (*asm_asm)(char *code);
int (*asm_func)(Node *root);
int (*asm_call)(int reg, Node *args);
void (*asm_preamble)(void);
void (*asm_postamble)(void);
