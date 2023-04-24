#ifndef __ASM16_
#define __ASM16_

/* UTILITY */
void asm16_stackfree(void);

/* LOADING */
int asm16_load(int val);
int asm16_loadstr(int id);
int asm16_loadglob(Sym *sym);
int asm16_loadlocl(Sym *sym);

/* STORING */
int asm16_storeglob(int reg, Sym *sym);
int asm16_storelocl(int reg, Sym *sym);
int asm16_storederef(int reg0, int reg1, Sym *sym);
int asm16_push(int reg);

/* ARITHMETIC OPERATIONS */
int asm16_add(int reg0, int reg1);
int asm16_sub(int reg0, int reg1);
int asm16_mul(int reg0, int reg1);
int asm16_div(int reg0, int reg1);
int asm16_mod(int reg0, int reg1);

/* OTHER OPERATIONS */
int asm16_neg(int reg);
int asm16_incdec(int reg, char *ins);

/* MEMORY OPERATIONS */
int asm16_addr(Sym *sym);
int asm16_deref(int reg);

/* LOGICAL OPERATIONS */
int asm16_lognot(int reg);
int asm16_logor(int reg0, int reg1);
int asm16_logand(int reg0, int reg1);

/* BITWISE OPERATIONS */
int asm16_bitnot(int reg);
int asm16_bitor(int reg0, int reg1);
int asm16_bitxor(int reg0, int reg1);
int asm16_bitand(int reg0, int reg1);
int asm16_lshift(int reg0, int reg1);
int asm16_rshift(int reg0, int reg1);

/* COMPLEX CONSTRUCTS */
int asm16_if(Node *root, int cmd);
int asm16_while(Node *root, int cmd);
int asm16_for(Node *root, int cmd);

/* FLOW CONTROL */
int asm16_break(void);
int asm16_continue(void);
int asm16_ret(int reg);
int asm16_goto(char *name);

/* OTHER */
void asm16_label(int l);
int asm16_ulabel(char *name);
int asm16_cmpset(int reg0, int reg1, char *ins);
void asm16_cmpz(int reg);
void asm16_jumpeq(int l);
void asm16_jump(int l);
int asm16_alloc(Sym *sym);

/* OTHER: 2 ELECTRIC BOOGALOO */
int asm16_asm(char *code);
int asm16_func(Node *root);
int asm16_call(int reg, Node *args);
void asm16_preamble(void);
void asm16_postamble(void);

#endif
