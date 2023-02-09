#include "includes.h"

void var_decl()
{
    token_t *t;

    next(&t); // data type
    next(&t); // identifier
    next(&t); // semicolon
}