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
