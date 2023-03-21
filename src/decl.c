#include "includes.h"

void typemod(token_t *type)
{
    token_t *t;

    while (tokseq(1, T_ASTERISK))
    {
        next(&t);

        if (isvaltype(type))
        {
            val2ptr(type);
        }
        else if (isptr(type))
        {
            ptr2dptr(type);
        }
    }
}

dlist_t *args()
{
    token_t *type, *ident, *tmp;
    dlist_t *local = malloc(sizeof(dlist_t));

    dl_init(local, sizeof(sym_t));

    next(&tmp); // (

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
        next(&tmp);   // , or )

        sym_t sym;
        sym.type = type->token;
        sym.class = C_LOCL;
        sym.size = 1;
        sym.name = *(char **)names->get[ident->val.id];

        sym.offs = 0;
        for (int i = 0; i < local->used; i++)
        {
            sym_t *tmp = local->get[i];
            sym.offs += type2size(tmp->type);
        }

        dl_add(local, &sym);

        if (tmp->token == T_RPAR)
        {
            break;
        }
    }

    return local;
}

asnode_t *var_decl(int class)
{
    int size = 1;
    token_t *type, *ident, *tmp, *len = NULL;

    next(&type); // data type

    // pointer modifiers
    typemod(type);

    next(&ident); // identifier
    if (tokseq(1, T_LSQBR))
    {
        next(&tmp); // [
        next(&len); // integer literal
        next(&tmp); // ]

        /*if (isvaltype(type))
        {
            val2ptr(type);
        }
        else if (isptr(type))
        {
            ptr2dptr(type);
        }*/
    }
    next(&tmp); // semicolon

    if (len)
    {
        size = len->val.i;
    }

    // global variable
    if (class == C_GLOB)
    {
        ident->val.id = addglob(type->token, class, *(char **)names->get[ident->val.id], size, NULL, NULL, NULL);
        return NULL;
    }

    // local variable
    ident->token = ST_ALLOC;
    ident->val.id = addlocl(type->token, class, *(char **)names->get[ident->val.id], size);
    return mknode(ident, NULL, NULL, NULL);
}

void func_decl(int class)
{
    char *name;
    sym_t *sym;
    dlist_t *local;
    asnode_t *root;
    token_t *type, *ident, *tmp;

    next(&type);   // data type
    typemod(type); // pointer modifiers

    next(&ident); // identifier
    ident->token = ST_FUNC;

    local = args(); // parse arguments

    // declaration
    if (tokseq(1, T_SEMICOLON))
    {
        next(&tmp);
        addglob(type->token, class, *(char **)names->get[ident->val.id], NULL, local->used, NULL, NULL);
        return;
    }

    // definition
    name = *((char **)names->get[ident->val.id]);
    ident->val.id = findglob(names->get[ident->val.id]);
    if (ident->val.id < 0)
    {
        ident->val.id = addglob(type->token, class, name, NULL, local->used, local, root);
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
        level = 0;
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
            if (tokseq(2, T_IDENT, T_LPAR))
            {
                back();
                func_decl(C_FUNC);
            }
            else
            {
                back();
                var_decl(C_GLOB);
            }
            break;
        case T_EXTERN:
            next(&t);
            next(&t);
            if (tokseq(2, T_IDENT, T_LPAR))
            {
                back();
                func_decl(C_EXTN);
            }
            else
            {
                back();
                var_decl(C_EXTN);
            }
            break;
        default:
            printf("ERROR: line %d\n", t->line);
            exit(1);
        }
    }

    return 0;
}
