#ifndef __LEX_
#define __LEX_

extern FILE *srcf;
extern dlist_t *tokens;

typedef struct
{
    int token;
    union
    {
        int i;
        int id;
        float f;
    } value;
    int line;
} token_t;

int next(token_t *t);
int back();
int tokseq(int n, ...);
void lex();

#endif
