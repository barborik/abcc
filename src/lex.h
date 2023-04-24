#ifndef __LEX_
#define __LEX_

struct tok
{
    int token;
    union
    {
        char    c;
        long    i;
        double  f;
    } val;
    int line;
};

int next(Tok **t);
int back(void);
void reset(void);
int tokseq(int n, ...);
void lex(void);

#endif
