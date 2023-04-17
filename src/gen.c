#include "includes.h"

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
    case A_ARGS: reginfo = &arg64; break;
    }

    // DEBUG
    fprintf(out_f, "\t; line %d\n", tok->line);

    asm_stackfree();

    switch (tok->token)
    {
    case ST_LEFT: return leftreg;
    case ST_RIGHT: return rightreg;
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
    case ST_ALLOC: return asm_alloc(sym);
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
    case ST_FOR: return asm_for(root, cmd);
    case ST_FUNC: return asm_func(root->left);
    case ST_CALL: return asm_call(leftreg, root->mid);
    case ST_RETURN: return asm_ret(leftreg);
    case ST_INC: return asm_incdec(leftreg, "inc");
    case ST_DEC: return asm_incdec(leftreg, "dec");
    }

    printf("ERROR: line %d, token: %d\n", tok->line, tok->token);
    exit(1);
}
