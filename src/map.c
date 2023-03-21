#include "includes.h"

int bin2stx(token_t *t)
{
    switch (t->token)
    {
    case T_PLUS:
        t->token = ST_ADD;
        break;
    case T_MINUS:
        t->token = ST_SUB;
        break;
    case T_ASTERISK:
        t->token = ST_MUL;
        break;
    case T_FSLASH:
        t->token = ST_DIV;
        break;
    case T_EQ:
        t->token = ST_EQ;
        break;
    case T_NE:
        t->token = ST_NE;
        break;
    case T_GT:
        t->token = ST_GT;
        break;
    case T_LT:
        t->token = ST_LT;
        break;
    case T_GE:
        t->token = ST_GE;
        break;
    case T_LE:
        t->token = ST_LE;
        break;
    case T_AMP:
        t->token = ST_BITAND;
        break;
    case T_DAMP:
        t->token = ST_LOGAND;
        break;
    case T_PIPE:
        t->token = ST_BITOR;
        break;
    case T_DPIPE:
        t->token = ST_LOGOR;
        break;
    case T_ASSIGN:
        t->token = ST_ASSIGN;
        break;
    case T_INTLIT:
        t->token = ST_INTLIT;
        break;
    case T_IDENT:
        t->token = ST_IDENT;
        idsym(t);
        break;
    default:
        return 0;
    }

    return 1;
}

int un2stx(token_t *t)
{
    switch (t->token)
    {
    case T_EXCL:
        t->token = ST_LOGNOT;
        break;
    case T_TILDA:
        t->token = ST_BITNOT;
        break;
    case T_AMP:
        t->token = ST_ADDR;
        break;
    case T_ASTERISK:
        t->token = ST_DEREF;
        break;
    case T_INTLIT:
        t->token = ST_INTLIT;
        break;
    case T_IDENT:
        t->token = ST_IDENT;
        idsym(t);
        break;
    default:
        return 0;
    }

    return 1;
}

int stmt2stx(token_t *t)
{
    switch (t->token)
    {
    case T_ASSIGN:
        t->token = ST_ASSIGN;
        break;
    case T_IF:
        t->token = ST_IF;
        break;
    case T_WHILE:
        t->token = ST_WHILE;
        break;
    default:
        return 0;
    }

    return 1;
}

int val2ptr(token_t *t)
{
    switch (t->token)
    {
    case T_I0:
        t->token = T_I0PTR;
        break;
    case T_I8:
        t->token = T_I8PTR;
        break;
    case T_I16:
        t->token = T_I16PTR;
        break;
    case T_I32:
        t->token = T_I32PTR;
        break;
    case T_I64:
        t->token = T_I64PTR;
        break;
    case T_U0:
        t->token = T_U0PTR;
        break;
    case T_U8:
        t->token = T_U8PTR;
        break;
    case T_U16:
        t->token = T_U16PTR;
        break;
    case T_U32:
        t->token = T_U32PTR;
        break;
    case T_U64:
        t->token = T_U64PTR;
        break;
    default:
        return 0;
    }

    return 1;
}

int ptr2val(token_t *t)
{
    switch (t->token)
    {
    case T_I0PTR:
        t->token = T_I0;
        break;
    case T_I8PTR:
        t->token = T_I8;
        break;
    case T_I16PTR:
        t->token = T_I16;
        break;
    case T_I32PTR:
        t->token = T_I32;
        break;
    case T_I64PTR:
        t->token = T_I64;
        break;
    case T_U0PTR:
        t->token = T_U0;
        break;
    case T_U8PTR:
        t->token = T_U8;
        break;
    case T_U16PTR:
        t->token = T_U16;
        break;
    case T_U32PTR:
        t->token = T_U32;
        break;
    case T_U64PTR:
        t->token = T_U64;
        break;
    default:
        return 0;
    }

    return 1;
}

int ptr2dptr(token_t *t)
{
    switch (t->token)
    {
    case T_I0PTR:
        t->token = T_I0DPTR;
        break;
    case T_I8PTR:
        t->token = T_I8DPTR;
        break;
    case T_I16PTR:
        t->token = T_I16DPTR;
        break;
    case T_I32PTR:
        t->token = T_I32DPTR;
        break;
    case T_I64PTR:
        t->token = T_I64DPTR;
        break;
    case T_U0PTR:
        t->token = T_U0DPTR;
        break;
    case T_U8PTR:
        t->token = T_U8DPTR;
        break;
    case T_U16PTR:
        t->token = T_U16DPTR;
        break;
    case T_U32PTR:
        t->token = T_U32DPTR;
        break;
    case T_U64PTR:
        t->token = T_U64DPTR;
        break;
    default:
        return 0;
    }

    return 1;
}

int type2size(int type)
{
    switch (type)
    {
    case T_I8:
    case T_U8:
        return 1;
    case T_I16:
    case T_U16:
        return 2;
    case T_I32:
    case T_U32:
        return 4;
    case T_I64:
    case T_U64:
        return 8;
    default:
        return 8;
    }

    return -1;
}
