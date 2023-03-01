#include "includes.h"

int line;
int last;
FILE *srcf;
dlist_t *tokens;
size_t tindex = 0;

// sets the passed token to the next token available
// returns 1 on success and 0 on failure
int next(token_t **t)
{
    if (tindex > tokens->used - 1)
    {
        return 0;
    }

    *t = ((token_t *)tokens->get[tindex]);
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

// resets the token iterator
void reset()
{
    tindex = 0;
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

// matches token sequence
int tokseq(int n, ...)
{
    token_t t;
    va_list list;
    va_start(list, n);

    for (int i = 0; i < n; i++)
    {
        next(&t);
        if (va_arg(list, int) != t.token)
        {
            for (int j = 0; j < i; i++)
            {
                back();
            }

            va_end(list);
            return 0;
        }
    }

    va_end(list);
    return 1;
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

void keyword(token_t *t)
{
    // load the keyword
    fseek(srcf, -1, SEEK_CUR);
    char kword[1024];
    int i = 0, c = fgetc(srcf);
    while (isalpha(c) || isdigit(c))
    {
        kword[i] = c;
        c = fgetc(srcf);
        i++;
    }
    fseek(srcf, -1, SEEK_CUR);
    kword[i] = 0;

    // match the keyword
    if (!strcmp(kword, "I8"))
    {
        t->token = T_I8;
        return;
    }

    if (!strcmp(kword, "I16"))
    {
        t->token = T_I16;
        return;
    }

    if (!strcmp(kword, "I32"))
    {
        t->token = T_I32;
        return;
    }

    if (!strcmp(kword, "I64"))
    {
        t->token = T_I64;
        return;
    }

    if (!strcmp(kword, "if"))
    {
        t->token = T_IF;
        return;
    }

    t->token = T_IDENT;
    if (findglob(kword) < 0)
    {
        addglob(last, kword);
    }
    t->value.id = findglob(kword);
    return;
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

    // other
    case '=':
        if (fgetc(srcf) == '=')
        {
            t->token = T_EQUALS;
            break;
        }
        fseek(srcf, -1, SEEK_CUR);

        t->token = T_ASSIGN;
        break;
    case ';':
        t->token = T_SEMICOLON;
        break;
    case '(':
        t->token = T_RPAR;
        break;
    case ')':
        t->token = T_LPAR;
        break;
    case '{':
        t->token = T_LBRACE;
        break;
    case '}':
        t->token = T_RBRACE;
        break;
    default:
        if (isdigit(c))
        {
            t->token = T_INTLIT;
            t->value.i = parse2i(c);
            break;
        }

        keyword(t);
        break;
    }

    t->line = line;
    return 1;
}

void lex()
{
    tokens = malloc(sizeof(dlist_t));
    dl_init(tokens, sizeof(token_t));

    token_t t;
    while (scan(&t))
    {
        last = t.token;
        dl_add(tokens, &t);
    }
}