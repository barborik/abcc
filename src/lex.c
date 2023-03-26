#include "includes.h"

int line = 1;
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
    token_t *t;
    va_list list;
    va_start(list, n);

    for (int i = 0; i < n; i++)
    {
        next(&t);
        if (va_arg(list, int) != t->token)
        {
            for (int j = 0; j < i + 1; j++)
            {
                back();
            }

            va_end(list);
            return 0;
        }
    }

    for (int i = 0; i < n; i++)
    {
        back();
    }

    va_end(list);
    return 1;
}

int escseq(int c)
{
    switch (c)
    {
    case '0':
        return '\0';
    case 'a':
        return '\a';
    case 'b':
        return '\b';
    case 'e':
        return '\e';
    case 'f':
        return '\f';
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    case 'v':
        return '\v';
    case '\\':
    case '\'':
    case '\"':
        return c;
    }

    return -1;
}

int parse2str()
{
    char str[512];

    int c = fgetc(srcf);
    for (int i = 0; c != '\"'; i++)
    {
        if (c == '\\')
        {
            c = fgetc(srcf);
            c = escseq(c);
        }

        str[i] = (char)c;
        str[i + 1] = 0;
        c = fgetc(srcf);
    }

    return adduniq(str);
}

char parse2char()
{
    char c = fgetc(srcf);

    if (c == '\\')
    {
        c = fgetc(srcf);
        c = escseq(c);
    }

    fgetc(srcf); // remove the ending quote
    return (char)c;
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

    // try to match the keyword
    if (match(kword, t))
    {
        return;
    }

    // no match found, store it as an identifier
    t->token = T_IDENT;
    t->val.id = adduniq(kword);
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
    case '>':
        if (fgetc(srcf) == '=')
        {
            t->token = T_GE;
            break;
        }
        fseek(srcf, -1, SEEK_CUR);

        t->token = T_GT;
        break;
    case '<':
        if (fgetc(srcf) == '=')
        {
            t->token = T_LE;
            break;
        }
        fseek(srcf, -1, SEEK_CUR);

        t->token = T_LT;
        break;
    case '!':
        if (fgetc(srcf) == '=')
        {
            t->token = T_NE;
            break;
        }
        fseek(srcf, -1, SEEK_CUR);

        t->token = T_EXCL;
        break;
    case '~':
        t->token = T_TILDA;
        break;
    case '&':
        if (fgetc(srcf) == '&')
        {
            t->token = T_DAMP;
            break;
        }
        fseek(srcf, -1, SEEK_CUR);

        t->token = T_AMP;
        break;
    case '|':
        if (fgetc(srcf) == '|')
        {
            t->token = T_DPIPE;
            break;
        }
        fseek(srcf, -1, SEEK_CUR);

        t->token = T_PIPE;
        break;
    // other
    case '=':
        if (fgetc(srcf) == '=')
        {
            t->token = T_EQ;
            break;
        }
        fseek(srcf, -1, SEEK_CUR);

        t->token = T_ASSIGN;
        break;
    case ';':
        t->token = T_SEMICOLON;
        break;
    case '(':
        t->token = T_LPAR;
        break;
    case ')':
        t->token = T_RPAR;
        break;
    case '[':
        t->token = T_LSQBR;
        break;
    case ']':
        t->token = T_RSQBR;
        break;
    case '{':
        t->token = T_LBRACE;
        break;
    case '}':
        t->token = T_RBRACE;
        break;
    case ',':
        t->token = T_COMMA;
        break;
    case '.':
        t->token = T_DOT;
        break;
    case '\'':
        t->token = T_CHARLIT;
        t->val.c = parse2char();
        break;
    case '\"':
        t->token = T_STRLIT;
        t->val.id = parse2str();
        break;
    default:
        if (isdigit(c))
        {
            t->token = T_INTLIT;
            t->val.i = parse2i();
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
        dl_add(tokens, &t);
    }
}