#include "includes.h"

int line;
FILE *srcf;
dlist_t tokens;
size_t tindex = 0;

// sets the passed token to the next token available
// returns 1 on success and 0 on failure
int next(token_t *t)
{
    if (tindex > tokens.used - 1)
    {
        return 0;
    }

    *t = *((token_t *)tokens.get[tindex]);
    tindex++;
    return 1;
}

// goes one token back in the tokens list
// returns 1 on success and 0 on failure
int back()
{
    if (tindex == 0)
    {
        return 0;
    }

    tindex--;
    return 1;
}

// returns the next non-whitespace character
int nextc()
{
    int c;
    do
    {
        c = fgetc(srcf);
        if (c == '\n')
        {
            line++;
        }
    } while (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f');

    return c;
}

// parses and returns next integer in the source file stream
// reader head must be on the first digit
int parse2i()
{
    fseek(srcf, -1, SEEK_CUR);
    int c = nextc(), v = 0;

    while (isdigit(c))
    {
        v = v * 10 + (c - '0');
        c = nextc();
    }

    fseek(srcf, -1, SEEK_CUR);
    return v;
}

// scans the next token
// returns 1 on success and 0 on failure
int scan(token_t *t)
{
    int c = nextc();
    switch (c)
    {
    case EOF:
        return 0;

    // operators
    case '+':
        t->token = T_PLUS;
        break;
    case '-':
        t->token = T_MINUS;
        break;
    case '*':
        t->token = T_ASTERISK;
        break;
    case '/':
        t->token = T_FSLASH;
        break;
    default:
        if (isdigit(c))
        {
            t->token = T_LITINT;
            t->value.i = parse2i(c);
        }
        break;
    }

    t->line = line;
    return 1;
}

void lex()
{
    dl_init(&tokens, sizeof(token_t));

    token_t t;
    while (scan(&t))
    {
        dl_add(&tokens, &t);
    }
}