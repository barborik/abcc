#include "includes.h"

int match(const char *kword, Tok *t)
{
    /* === TYPES === */

    // value types
    if (!strcmp(kword, "I0"))
    {
        t->token = LT_I0;
        return 1;
    }

    if (!strcmp(kword, "I8"))
    {
        t->token = LT_I8;
        return 1;
    }

    if (!strcmp(kword, "I16"))
    {
        t->token = LT_I16;
        return 1;
    }

    if (!strcmp(kword, "I32"))
    {
        t->token = LT_I32;
        return 1;
    }

    if (!strcmp(kword, "I64"))
    {
        t->token = LT_I64;
        return 1;
    }

    if (!strcmp(kword, "U0"))
    {
        t->token = LT_U0;
        return 1;
    }

    if (!strcmp(kword, "U8"))
    {
        t->token = LT_U8;
        return 1;
    }

    if (!strcmp(kword, "U16"))
    {
        t->token = LT_U16;
        return 1;
    }

    if (!strcmp(kword, "U32"))
    {
        t->token = LT_U32;
        return 1;
    }

    if (!strcmp(kword, "U64"))
    {
        t->token = LT_U64;
        return 1;
    }

    /* === BLOCK === */
    if (!strcmp(kword, "if"))
    {
        t->token = LT_IF;
        return 1;
    }

    if (!strcmp(kword, "else"))
    {
        t->token = LT_ELSE;
        return 1;
    }

    if (!strcmp(kword, "while"))
    {
        t->token = LT_WHILE;
        return 1;
    }

    if (!strcmp(kword, "for"))
    {
        t->token = LT_FOR;
        return 1;
    }

    /* === OTHER === */
    if (!strcmp(kword, "return"))
    {
        t->token = LT_RETURN;
        return 1;
    }

    if (!strcmp(kword, "extern"))
    {
        t->token = LT_EXTERN;
        return 1;
    }

    if (!strcmp(kword, "break"))
    {
        t->token = LT_BREAK;
        return 1;
    }

    if (!strcmp(kword, "continue"))
    {
        t->token = LT_CONTINUE;
        return 1;
    }

    if (!strcmp(kword, "goto"))
    {
        t->token = LT_GOTO;
        return 1;
    }

    if (!strcmp(kword, "typedef"))
    {
        t->token = LT_TDEF;
        return 1;
    }

    if (!strcmp(kword, "sizeof"))
    {
        t->token = LT_SIZEOF;
        return 1;
    }

    /* INLINE ASSEMBLY */
    if (!strcmp(kword, "__asm"))
    {
        int c, i = 0;
        char code[4096];

        nextc(); // {
        c = fgetc(src_f);

        while (c != '}')
        {
            code[i] = c;
            c = fgetc(src_f);
            i++;
        }
        code[i] = 0;

        char *final = malloc(i + 1);
        strcpy(final, code);

        t->token = LT_ASM;
        t->val.i = adduniq(final);

        return 1;
    }

    return 0;
}