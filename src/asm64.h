#ifndef __ASM64_
#define __ASM64_

int ralloc(void);
void rfree(int reg);
void rfree_all(void);
int asm_load(int val);
int asm_loadstr(int id);
int asm_add(int reg0, int reg1);
int asm_sub(int reg0, int reg1);
int asm_mul(int reg0, int reg1);
int asm_div(int reg0, int reg1);
int asm_lognot(int reg);
int asm_bitnot(int reg);
int asm_neg(int reg);
int asm_bitor(int reg0, int reg1);
int asm_logor(int reg0, int reg1);
int asm_bitand(int reg0, int reg1);
int asm_logand(int reg0, int reg1);
int asm_bitxor(int reg0, int reg1);
int asm_lshift(int reg0, int reg1);
int asm_rshift(int reg0, int reg1);
int asm_addr(Sym *sym);
int asm_deref(int reg);
int asm_loadglob(Sym *sym);
int asm_storeglob(int reg, Sym *sym);
int asm_loadlocl(Sym *sym);
int asm_storelocl(int reg, Sym *sym);
int asm_storederef(int reg0, int reg1, Sym *sym);
void asm_label(int l);
int asm_cmpset(int reg0, int reg1, char *ins);
void asm_cmpz(int reg);
void asm_jumpeq(int l);
void asm_jump(int l);
int asm_alloc(Sym *sym);
int asm_func(Node *root);
int asm_call(int reg, Node *args);
int asm_ret(int reg);
void asm_stackfree(void);
int asm_break(void);
int asm_continue(void);
int asm_if(Node *root, int cmd);
int asm_while(Node *root, int cmd);
int asm_for(Node *root, int cmd);
int asm_incdec(int reg, char *ins);
int asm_ulabel(char *name);
int asm_goto(char *name);
void asm_preamble(void);
void asm_postamble(void);

#endif
