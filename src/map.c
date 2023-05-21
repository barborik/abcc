#include "includes.h"

int bin2stx(Tok *t)
{
    switch (t->token)
    {
    case LT_PLUS:
        t->token = ST_ADD;
        break;
    case LT_MINUS:
        t->token = ST_SUB;
        break;
    case LT_ASTERISK:
        t->token = ST_MUL;
        break;
    case LT_FSLASH:
        t->token = ST_DIV;
        break;
    case LT_PERCENT:
        t->token = ST_MOD;
        break;
    case LT_EQ:
        t->token = ST_EQ;
        break;
    case LT_NE:
        t->token = ST_NE;
        break;
    case LT_GT:
        t->token = ST_GT;
        break;
    case LT_LT:
        t->token = ST_LT;
        break;
    case LT_GE:
        t->token = ST_GE;
        break;
    case LT_LE:
        t->token = ST_LE;
        break;
    case LT_AMP:
        t->token = ST_BITAND;
        break;
    case LT_DAMP:
        t->token = ST_LOGAND;
        break;
    case LT_PIPE:
        t->token = ST_BITOR;
        break;
    case LT_DPIPE:
        t->token = ST_LOGOR;
        break;
    case LT_CARET:
        t->token = ST_BITXOR;
        break;
    case LT_LSHIFT:
        t->token = ST_LSHIFT;
        break;
    case LT_RSHIFT:
        t->token = ST_RSHIFT;
        break;
    case LT_ASSIGN:
        t->token = ST_ASSIGN;
        break;
    default:
        return 0;
    }

    return 1;
}

int un2stx(Tok *t)
{
    switch (t->token)
    {
    case LT_INTLIT:
        t->token = ST_INTLIT;
        break;
    case LT_FLOATLIT:
        t->token = ST_FLOATLIT;
        break;
    case LT_CHARLIT:
        t->token = ST_CHARLIT;
        break;
    case LT_STRLIT:
        t->token = ST_STRLIT;
        break;
    case LT_IDENT:
        t->token = ST_IDENT;
        break;
    case LT_SIZEOF:
        t->token = ST_SIZEOF;
        break;
    default:
        return 0;
    }

    return 1;
}

int type2size(Type type)
{
    if (type.addr)
    {
        return 8;
    }

    switch (type.type)
    {
    case LT_I8:
    case LT_U8:
        return 1;
    case LT_I16:
    case LT_U16:
        return 2;
    case LT_I32:
    case LT_U32:
        return 4;
    case LT_I64:
    case LT_U64:
        return 8;
    }

    return 0;
}

void bitmode(int m)
{
    mode = m;

    switch (m)
    {
    case M_16:
        reginfo = &std16;

        asm_stackfree = asm16_stackfree;
        asm_load = asm16_load;
        asm_loadstr = asm16_loadstr;
        asm_loadglob = asm16_loadglob;
        asm_loadlocl = asm16_loadlocl;
        asm_storeglob = asm16_storeglob;
        asm_storelocl = asm16_storelocl;
        asm_storederef = asm16_storederef;
        asm_push = asm16_push;
        asm_add = asm16_add;
        asm_sub = asm16_sub;
        asm_mul = asm16_mul;
        asm_div = asm16_div;
        asm_mod = asm16_mod;
        asm_neg = asm16_neg;
        asm_incdec = asm16_incdec;
        asm_addr = asm16_addr;
        asm_deref = asm16_deref;
        asm_lognot = asm16_lognot;
        asm_logor = asm16_logor;
        asm_logand = asm16_logand;
        asm_bitnot = asm16_bitnot;
        asm_bitor = asm16_bitor;
        asm_bitxor = asm16_bitxor;
        asm_bitand = asm16_bitand;
        asm_lshift = asm16_lshift;
        asm_rshift = asm16_rshift;
        asm_if = asm16_if;
        asm_while = asm16_while;
        asm_for = asm16_for;
        asm_break = asm16_break;
        asm_continue = asm16_continue;
        asm_ret = asm16_ret;
        asm_goto = asm16_goto;
        asm_label = asm16_label;
        asm_ulabel = asm16_ulabel;
        asm_cmpset = asm16_cmpset;
        asm_cmpz = asm16_cmpz;
        asm_jumpeq = asm16_jumpeq;
        asm_jump = asm16_jump;
        asm_alloc = asm16_alloc;
        asm_asm = asm16_asm;
        asm_func = asm16_func;
        asm_call = asm16_call;
        asm_preamble = asm16_preamble;
        asm_postamble = asm16_postamble;
        break;
    case M_64:
        reginfo = &std64;

        asm_stackfree = asm64_stackfree;
        asm_load = asm64_load;
        asm_loadstr = asm64_loadstr;
        asm_loadglob = asm64_loadglob;
        asm_loadlocl = asm64_loadlocl;
        asm_storeglob = asm64_storeglob;
        asm_storelocl = asm64_storelocl;
        asm_storederef = asm64_storederef;
        asm_push = asm64_push;
        asm_add = asm64_add;
        asm_sub = asm64_sub;
        asm_mul = asm64_mul;
        asm_div = asm64_div;
        asm_mod = asm64_mod;
        asm_neg = asm64_neg;
        asm_incdec = asm64_incdec;
        asm_addr = asm64_addr;
        asm_deref = asm64_deref;
        asm_lognot = asm64_lognot;
        asm_logor = asm64_logor;
        asm_logand = asm64_logand;
        asm_bitnot = asm64_bitnot;
        asm_bitor = asm64_bitor;
        asm_bitxor = asm64_bitxor;
        asm_bitand = asm64_bitand;
        asm_lshift = asm64_lshift;
        asm_rshift = asm64_rshift;
        asm_if = asm64_if;
        asm_while = asm64_while;
        asm_for = asm64_for;
        asm_break = asm64_break;
        asm_continue = asm64_continue;
        asm_ret = asm64_ret;
        asm_goto = asm64_goto;
        asm_label = asm64_label;
        asm_ulabel = asm64_ulabel;
        asm_cmpset = asm64_cmpset;
        asm_cmpz = asm64_cmpz;
        asm_jumpeq = asm64_jumpeq;
        asm_jump = asm64_jump;
        asm_alloc = asm64_alloc;
        asm_asm = asm16_asm;
        asm_func = asm64_func;
        asm_call = asm64_call;
        asm_preamble = asm64_preamble;
        asm_postamble = asm64_postamble;
        break;
    }
}