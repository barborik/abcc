#include "includes.h"

int match(const char *kword, token_t *t)
{
    /* === TYPES === */

    // value types
    if (!strcmp(kword, "I0"))
    {
        t->token = T_I0;
        return 1;
    }

    if (!strcmp(kword, "I8"))
    {
        t->token = T_I8;
        return 1;
    }

    if (!strcmp(kword, "I16"))
    {
        t->token = T_I16;
        return 1;
    }

    if (!strcmp(kword, "I32"))
    {
        t->token = T_I32;
        return 1;
    }

    if (!strcmp(kword, "I64"))
    {
        t->token = T_I64;
        return 1;
    }

    if (!strcmp(kword, "U0"))
    {
        t->token = T_U0;
        return 1;
    }

    if (!strcmp(kword, "U8"))
    {
        t->token = T_U8;
        return 1;
    }

    if (!strcmp(kword, "U16"))
    {
        t->token = T_U16;
        return 1;
    }

    if (!strcmp(kword, "U32"))
    {
        t->token = T_U32;
        return 1;
    }

    if (!strcmp(kword, "U64"))
    {
        t->token = T_U64;
        return 1;
    }

    /* === BLOCK === */
    if (!strcmp(kword, "if"))
    {
        t->token = T_IF;
        return 1;
    }

    if (!strcmp(kword, "while"))
    {
        t->token = T_WHILE;
        return 1;
    }

    /* === OTHER === */
    if (!strcmp(kword, "return"))
    {
        t->token = T_RETURN;
        return 1;
    }

    if (!strcmp(kword, "extern"))
    {
        t->token = T_EXTERN;
        return 1;
    }

    return 0;
}