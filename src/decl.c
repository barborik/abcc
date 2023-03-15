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

dlist_t *fargs()
{
    token_t *type, *ident, *t;
    dlist_t *args = malloc(sizeof(dlist_t));
    dl_init(args, sizeof(sym_t));

    next(&t); // (

    while (1)
    {
        next(&type); // data type
        typemod(type);

        if (type->token == T_U0 || type->token == T_I0)
        {
            next(&type); // )
            break;
        }

        next(&ident); // ident
        next(&t);     // , or )

        sym_t sym;
        sym.type = type->token;
        sym.name = *(char **)names->get[ident->val.id];

        dl_add(args, &sym);

        if (t->token == T_RPAR)
        {
            break;
        }
    }

    return args;
}

void var_decl()
{
    token_t *type, *ident, *t;

    next(&type); // data type

    // pointer modifiers
    typemod(type);

    next(&ident); // identifier
    next(&t);     // semicolon

    ident->val.id = addglob(type->token, C_VAR, *(char **)names->get[ident->val.id], NULL, NULL, NULL);
}

void func_decl()
{
    dlist_t *args;
    asnode_t *root;
    token_t *type, *ident, *t;

    next(&type);   // data type
    typemod(type); // pointer modifiers

    next(&ident); // identifier
    ident->token = ST_FUNC;

    args = fargs();
    root = mknode(ident, block_stmt(), NULL, NULL);

    dlist_t *local = malloc(sizeof(dlist_t));
    dl_init(local, sizeof(sym_t));

    ident->val.id = addglob(type->token, C_FUNC, *(char **)names->get[ident->val.id], args, local, root);
}
