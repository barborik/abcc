#include "includes.h"

void typemod(token_t *type)
{
    token_t *t;

    for (int i = 0; i < 2; i++)
    {
        next(&t);
        if (t->token == T_ASTERISK)
        {
            if (isvaltype(t))
            {
                val2ptr(type);
            }

            if (isptr(t))
            {
                ptr2dptr(type);
            }
        }
        else
        {
            back();
        }
    }
}

void var_decl()
{
    token_t *type, *ident, *t;

    next(&type); // data type

    // pointer modifiers
    typemod(type);

    next(&ident); // identifier
    next(&t);     // semicolon

    sym_t *sym = glob->get[ident->value.id];
    sym->type = type->token;
}

void func_decl()
{
    token_t *type, *ident, *t;

    next(&type); // data type

    // pointer modifiers
    typemod(type);

    next(&ident); // identifier

    sym_t *sym = glob->get[ident->value.id];
    sym->type = type->token;
    sym->func = 1;
    ident->token = ST_FUNC;

    next(&t); // (
    next(&t); // U0
    next(&t); // )

    sym->root = mknode(ident, block_stmt(), NULL, NULL);
}
