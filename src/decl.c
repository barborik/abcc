#include "includes.h"

void var_decl()
{
    token_t *type, *ident, *t;

    next(&type); // data type

    // pointer modifiers
    for (int i = 0; i < 2; i++)
    {
        next(&t);
        if (t->token == T_ASTERISK)
        {
            val2ptr(type);
        }
        else
        {
            back();
        }
    }

    next(&ident); // identifier
    next(&t);     // semicolon

    ((sym_t *)glob->get[ident->value.id])->type = type->token;
}