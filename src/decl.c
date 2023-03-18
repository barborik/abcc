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

        sym.offs = 0;
        for (int i = 0; i < args->used; i++)
        {
            sym_t *arg = args->get[i];
            sym.offs += type2size(arg->type);
        }

        dl_add(args, &sym);

        if (t->token == T_RPAR)
        {
            break;
        }
    }

    return args;
}

void var_decl(int class, sym_t *func)
{
    sym_t sym;
    token_t *type, *ident, *t;

    next(&type); // data type

    // pointer modifiers
    typemod(type);

    next(&ident); // identifier
    next(&t);     // semicolon

    // global variable
    if (class == C_GLOB)
    {
        ident->val.id = addglob(type->token, class, *(char **)names->get[ident->val.id], NULL, NULL, NULL);
        return;
    }

    // local variable
    sym.name = *(char **)names->get[ident->val.id];
    sym.type = ident->token;
    sym.class = class;

    sym.offs = 0;
    for (int i = 0; i < func->local->used; i++)
    {
        sym_t *local = func->local->get[i];
        sym.offs += type2size(local->type);
    }

    dl_add(func->local, &sym);
}

void func_decl(int class)
{
    char *name;
    sym_t *sym;
    dlist_t *local;
    asnode_t *root;
    token_t *type, *ident, *t;

    next(&type);   // data type
    typemod(type); // pointer modifiers

    next(&ident); // identifier
    ident->token = ST_FUNC;

    local = fargs(); // arguments

    // declaration
    if (tokseq(1, T_SEMICOLON))
    {
        next(&t);
        addglob(type->token, class, *(char **)names->get[ident->val.id], local->used, NULL, NULL);
        return;
    }

    // definition
    name = *((char **)names->get[ident->val.id]);
    ident->val.id = findglob(names->get[ident->val.id]);
    if (ident->val.id < 0)
    {
        ident->val.id = addglob(type->token, class, name, local->used, local, root);
    }

    func = glob->get[ident->val.id];
    root = mknode(ident, block_stmt(glob->get[ident->val.id]), NULL, NULL);

    sym = glob->get[ident->val.id];
    sym->root = root;
}

void decl()
{
    token_t *t;
    while (next(&t))
    {
        // printf("decl | %d %d\n", t->line, t->token);
        back();

        switch (t->token)
        {
        case T_I0:
        case T_I8:
        case T_I16:
        case T_I32:
        case T_I64:
        case T_U0:
        case T_U8:
        case T_U16:
        case T_U32:
        case T_U64:
            next(&t);
            if (tokseq(2, T_IDENT, T_SEMICOLON))
            {
                back();
                var_decl(C_GLOB, NULL);
            }
            else
            {
                back();
                func_decl(C_FUNC);
            }
            break;
        case T_EXTERN:
            next(&t);
            next(&t);
            if (tokseq(2, T_IDENT, T_SEMICOLON))
            {
                back();
                var_decl(C_EXTN, NULL);
            }
            else
            {
                back();
                func_decl(C_EXTN);
            }
            break;
        }
    }

    return 0;
}
