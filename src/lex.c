#include "includes.h"

int line = 1;
char *file;

dlist_t *tokens;
size_t tindex = 0;

enum
{
    S_PLUS = 1,
    S_MINUS = 0,
};

// sets the passed token to the next token available
// returns 1 on success and 0 on failure
int next(Tok **t)
{
    if (tindex > tokens->used - 1)
    {
        return 0;
    }

    if (!t)
    {
        tindex++;
        return 1;
    }

    *t = ((Tok *)tokens->get[tindex]);
    tindex++;
    return 1;
}

// goes one token back in the tokens list
// returns 1 on success and 0 on failure
int back(void)
{
    if (tindex == 0)
    {
        return 0;
    }

    tindex--;
    return 1;
}

// resets the token iterator
void reset(void)
{
    tindex = 0;
}

// returns the next non-whitespace character
int nextc()
{
    int c;
    char info[256];

    do
    {
        c = fgetc(src_f);

        switch (c)
        {
        case '#':
            fgets(info, 256, src_f);
            sscanf(info, " %d \"%[^\"]\"", &line, file);
            break;
        case '\n':
            line++;
            break;
        }

    } while (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '#');

    return c;
}

// matches token sequence
int tokseq(int n, ...)
{
    Tok *tok;
    va_list list;
    va_start(list, n);

    for (int i = 0; i < n; i++)
    {
        next(&tok);
        if (va_arg(list, int) != tok->token)
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

    int c = fgetc(src_f);
    for (int i = 0; c != '\"'; i++)
    {
        if (c == '\\')
        {
            c = fgetc(src_f);
            c = escseq(c);
        }

        str[i] = (char)c;
        str[i + 1] = 0;
        c = fgetc(src_f);
    }

    char *final = malloc(strlen(str));
    strcpy(final, str);

    dl_add(uniq, &final);
    return uniq->used - 1;
}

char parse2char()
{
    char c = fgetc(src_f);

    if (c == '\\')
    {
        c = fgetc(src_f);
        c = escseq(c);
    }

    fgetc(src_f); // remove the ending quote
    return (char)c;
}

// parses and returns next integer in the source file stream
// reader head must be on the first digit
int parse2i(int sign)
{
    fseek(src_f, -1, SEEK_CUR);
    int c = nextc(), v = 0;

    while (isdigit(c))
    {
        v = v * 10 + (c - '0');
        c = nextc();
    }

    fseek(src_f, -1, SEEK_CUR);
    return (sign) ? (v) : (-v);
}

// parses a numeric literal
void numeric(Tok *t)
{
    char num[64];
    int c = fgetc(src_f), i = 0, isfloat = 0;

    /*t->token = LT_INTLIT;
    t->val.i = parse2i(S_PLUS);*/

    while (isalnum(c) || c == '.')
    {
        if (c == '.')
        {
            isfloat = 1;
        }
        num[i] = c;
        c = fgetc(src_f);
        i++;
    }
    fseek(src_f, -1, SEEK_CUR);
    num[i] = 0;

    if (isfloat)
    {
        t->token = LT_FLOATLIT;
        t->val.f = strtof(num, NULL);
        return;
    }

    if (num[0] == '0')
    {
        switch (num[1])
        {
        case 'x':
            t->token = LT_INTLIT;
            t->val.i = strtol(num + 2 * sizeof(char), NULL, 16);
            return;
        case 'b':
            t->token = LT_INTLIT;
            t->val.i = strtol(num + 2 * sizeof(char), NULL, 2);
            return;
        }
    }

    t->token = LT_INTLIT;
    t->val.i = strtol(num, NULL, 10);
}

void keyword(Tok *t)
{
    // load the keyword
    fseek(src_f, -1, SEEK_CUR);
    char kword[256];
    int i = 0, c = fgetc(src_f);
    while (isalpha(c) || isdigit(c) || c == '_')
    {
        kword[i] = c;
        c = fgetc(src_f);
        i++;
    }
    fseek(src_f, -1, SEEK_CUR);
    kword[i] = 0;

    // try to match the keyword
    if (match(kword, t))
    {
        return;
    }

    // no match found, store it as an identifier
    t->token = LT_IDENT;
    t->val.i = adduniq(kword);
    return;
}

// scans the next token
// returns 1 on success and 0 on failure
int scan(Tok *t)
{
    int c = nextc();
    switch (c)
    {
    case EOF:
        return 0;

    /* OPERATORS */
    case '+':
        /*c = fgetc(src_f);
        if (isdigit(c))
        {
            t->token = LT_INTLIT;
            t->val.i = parse2i(S_PLUS);
            break;
        }
        fseek(src_f, -1, SEEK_CUR);*/

        if (fgetc(src_f) == '+')
        {
            t->token = LT_DPLUS;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        t->token = LT_PLUS;
        break;
    case '-':
        /*c = fgetc(src_f);
        if (isdigit(c))
        {
            t->token = LT_INTLIT;
            t->val.i = parse2i(S_MINUS);
            break;
        }
        fseek(src_f, -1, SEEK_CUR);*/

        if (fgetc(src_f) == '-')
        {
            t->token = LT_DMINUS;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        t->token = LT_MINUS;
        break;
    case '*':
        t->token = LT_ASTERISK;
        break;
    case '/':
        t->token = LT_FSLASH;
        break;
    case '%':
        t->token = LT_PERCENT;
        break;
    case '>':
        if (fgetc(src_f) == '=')
        {
            t->token = LT_GE;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        if (fgetc(src_f) == '>')
        {
            t->token = LT_RSHIFT;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        t->token = LT_GT;
        break;
    case '<':
        if (fgetc(src_f) == '=')
        {
            t->token = LT_LE;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        if (fgetc(src_f) == '<')
        {
            t->token = LT_LSHIFT;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        t->token = LT_LT;
        break;
    case '!':
        if (fgetc(src_f) == '=')
        {
            t->token = LT_NE;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        t->token = LT_EXCL;
        break;
    case '~':
        t->token = LT_TILDA;
        break;
    case '^':
        t->token = LT_CARET;
        break;
    case '&':
        if (fgetc(src_f) == '&')
        {
            t->token = LT_DAMP;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        t->token = LT_AMP;
        break;
    case '|':
        if (fgetc(src_f) == '|')
        {
            t->token = LT_DPIPE;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        t->token = LT_PIPE;
        break;

    /* OTHER */
    case '=':
        if (fgetc(src_f) == '=')
        {
            t->token = LT_EQ;
            break;
        }
        fseek(src_f, -1, SEEK_CUR);

        t->token = LT_ASSIGN;
        break;
    case '@':
        t->token = LT_ATMARK;
        break;
    case ':':
        t->token = LT_COLON;
        break;
    case ';':
        t->token = LT_SEMICOLON;
        break;
    case '(':
        t->token = LT_LPAR;
        break;
    case ')':
        t->token = LT_RPAR;
        break;
    case '[':
        t->token = LT_LSQBR;
        break;
    case ']':
        t->token = LT_RSQBR;
        break;
    case '{':
        t->token = LT_LBRACE;
        break;
    case '}':
        t->token = LT_RBRACE;
        break;
    case ',':
        t->token = LT_COMMA;
        break;
    case '.':
        t->token = LT_DOT;
        break;
    case '\'':
        t->token = LT_CHARLIT;
        t->val.i = parse2char();
        break;
    case '\"':
        t->token = LT_STRLIT;
        t->val.i = parse2str();
        break;
    case '$':
        numeric(t);
        break;
    default:
        keyword(t);
        break;
    }

    t->line = line;
    return 1;
}

void lex(void)
{
    file = malloc(256);
    tokens = malloc(sizeof(dlist_t));
    dl_init(tokens, sizeof(Tok));

    Tok t;
    while (scan(&t))
    {
        dl_add(tokens, &t);
    }
}