#ifndef __ASM64_
#define __ASM64_

/* UTILITY */
void asm64_stackfree(void);

/* LOADING */
int asm64_load(int val);
int asm64_loadstr(int id);
int asm64_loadglob(Sym *sym);
int asm64_loadlocl(Sym *sym);

/* STORING */
int asm64_storeglob(int reg, Sym *sym);
int asm64_storelocl(int reg, Sym *sym);
int asm64_storederef(int reg0, int reg1, Sym *sym);

/* ARITHMETIC OPERATIONS */
int asm64_add(int reg0, int reg1);
int asm64_sub(int reg0, int reg1);
int asm64_mul(int reg0, int reg1);
int asm64_div(int reg0, int reg1);
int asm64_mod(int reg0, int reg1);

/* OTHER OPERATIONS */
int asm64_neg(int reg);
int asm64_incdec(int reg, char *ins);

/* MEMORY OPERATIONS */
int asm64_addr(Sym *sym);
int asm64_deref(int reg);

/* LOGICAL OPERATIONS */
int asm64_lognot(int reg);
int asm64_logor(int reg0, int reg1);
int asm64_logand(int reg0, int reg1);

/* BITWISE OPERATIONS */
int asm64_bitnot(int reg);
int asm64_bitor(int reg0, int reg1);
int asm64_bitxor(int reg0, int reg1);
int asm64_bitand(int reg0, int reg1);
int asm64_lshift(int reg0, int reg1);
int asm64_rshift(int reg0, int reg1);

/* COMPLEX CONSTRUCTS */
int asm64_if(Node *root, int cmd);
int asm64_while(Node *root, int cmd);
int asm64_for(Node *root, int cmd);

/* FLOW CONTROL */
int asm64_break(void);
int asm64_continue(void);
int asm64_ret(int reg);
int asm64_goto(char *name);

/* OTHER */
void asm64_label(int l);
int asm64_ulabel(char *name);
int asm64_cmpset(int reg0, int reg1, char *ins);
void asm64_cmpz(int reg);
void asm64_jumpeq(int l);
void asm64_jump(int l);
int asm64_alloc(Sym *sym);

/* OTHER: 2 ELECTRIC BOOGALOO */
int asm64_func(Node *root);
int asm64_call(int reg, Node *args);
void asm64_preamble(void);
void asm64_postamble(void);

#endif
