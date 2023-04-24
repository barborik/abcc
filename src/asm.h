#ifndef __ASM_
#define __ASM_

extern int start, end;
extern int label, top, pushed;

/* UTILITY */
int ralloc(void);     /* SHARED */
void rfree(int reg);  /* SHARED */
void rfree_all(void); /* SHARED */
extern void (*asm_stackfree)(void);

/* LOADING */
extern int (*asm_load)(int val);
extern int (*asm_loadstr)(int id);
extern int (*asm_loadglob)(Sym *sym);
extern int (*asm_loadlocl)(Sym *sym);

/* STORING */
extern int (*asm_storeglob)(int reg, Sym *sym);
extern int (*asm_storelocl)(int reg, Sym *sym);
extern int (*asm_storederef)(int reg0, int reg1, Sym *sym);
extern int (*asm_push)(int reg);

/* ARITHMETIC OPERATIONS */
extern int (*asm_add)(int reg0, int reg1);
extern int (*asm_sub)(int reg0, int reg1);
extern int (*asm_mul)(int reg0, int reg1);
extern int (*asm_div)(int reg0, int reg1);
extern int (*asm_mod)(int reg0, int reg1);

/* OTHER OPERATIONS */
extern int (*asm_neg)(int reg);
extern int (*asm_incdec)(int reg, char *ins);

/* MEMORY OPERATIONS */
extern int (*asm_addr)(Sym *sym);
extern int (*asm_deref)(int reg);

/* LOGICAL OPERATIONS */
extern int (*asm_lognot)(int reg);
extern int (*asm_logor)(int reg0, int reg1);
extern int (*asm_logand)(int reg0, int reg1);

/* BITWISE OPERATIONS */
extern int (*asm_bitnot)(int reg);
extern int (*asm_bitor)(int reg0, int reg1);
extern int (*asm_bitxor)(int reg0, int reg1);
extern int (*asm_bitand)(int reg0, int reg1);
extern int (*asm_lshift)(int reg0, int reg1);
extern int (*asm_rshift)(int reg0, int reg1);

/* COMPLEX CONSTRUCTS */
extern int (*asm_if)(Node *root, int cmd);
extern int (*asm_while)(Node *root, int cmd);
extern int (*asm_for)(Node *root, int cmd);

/* FLOW CONTROL */
extern int (*asm_break)(void);
extern int (*asm_continue)(void);
extern int (*asm_ret)(int reg);
extern int (*asm_goto)(char *name);

/* OTHER */
extern void (*asm_label)(int l);
extern int (*asm_ulabel)(char *name);
extern int (*asm_cmpset)(int reg0, int reg1, char *ins);
extern void (*asm_cmpz)(int reg);
extern void (*asm_jumpeq)(int l);
extern void (*asm_jump)(int l);
extern int (*asm_alloc)(Sym *sym);

/* OTHER: 2 ELECTRIC BOOGALOO */
extern int (*asm_asm)(char *code);
extern int (*asm_func)(Node *root);
extern int (*asm_call)(int reg, Node *args);
extern void (*asm_preamble)(void);
extern void (*asm_postamble)(void);

#endif
