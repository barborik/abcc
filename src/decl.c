#include "includes.h"

void typemod(token_t *type)
{
    token_t *t;

    while (tokseq(1, T_ASTERISK))
    {
        next(&t);

        if (isvaltype(type->token))
        {
            val2ptr(type);
        }
        else if (isptr(type->token))
        {
            ptr2dptr(type);
        }
    }
}

int args(dlist_t *local)
{
    int argc = 0;
    token_t *type, *ident, *tmp;

    next(&tmp); // (

    while (1)
    {
        if (tokseq(3, T_DOT, T_DOT, T_DOT))
        {
            next(&tmp); // .
            next(&tmp); // .
            next(&tmp); // .
            argc = -1;
            break;
        }

        next(&type); // data type
        typemod(type);

        if (type->token == T_U0 || type->token == T_I0)
        {
            break;
        }

        next(&ident); // ident

        sym_t sym;
        sym.type = type->token;
        sym.class = C_LOCL;
        sym.size = 1;
        sym.name = *(char **)uniq->get[ident->val.id];
        sym.offs = 0;

        dl_add(local, &sym);
        argc++;

        if (tokseq(1, T_RPAR))
        {
            break;
        }
        next(&tmp); // ,
    }

    next(&tmp); // )
    return argc;
}

asnode_t *var_decl(int class)
{
    int size = 1, prim = 1;
    token_t *type, *ident, *tmp;

    next(&type); // data type

    // pointer modifiers
    typemod(type);

    next(&ident); // identifier
    if (tokseq(1, T_LSQBR))
    {
        next(&tmp); // [
        size = interpret(binexp(0));
        next(&tmp); // ]
        prim = 0;
    }
    next(&tmp); // semicolon

    // global variable
    if (class == C_GLOB || class == C_EXTN)
    {
        ident->val.id = addglob(type->token, class, *(char **)uniq->get[ident->val.id], size, NULL, NULL, NULL);
        ((sym_t *)glob->get[ident->val.id])->prim = prim;
        return NULL;
    }

    // local variable
    ident->token = ST_ALLOC;
    ident->val.id = addlocl(type->token, class, *(char **)uniq->get[ident->val.id], size);
    ((sym_t *)func->local->get[ident->val.id])->prim = prim;
    return mknode(ident, NULL, NULL, NULL);
}

void func_decl(int class)
{
    int argc;
    char *name;
    sym_t *sym;
    dlist_t *local;
    asnode_t *root;
    token_t *type, *ident, *tmp;

    next(&type);   // data type
    typemod(type); // pointer modifiers

    next(&ident); // identifier
    ident->token = ST_FUNC;

    local = malloc(sizeof(dlist_t));
    dl_init(local, sizeof(sym_t));
    argc = args(local); // parse arguments

    // declaration
    if (tokseq(1, T_SEMICOLON))
    {
        next(&tmp);
        addglob(type->token, class, *(char **)uniq->get[ident->val.id], NULL, argc, NULL, NULL);
        return;
    }

    // definition
    name = *((char **)uniq->get[ident->val.id]);
    ident->val.id = findglob(uniq->get[ident->val.id]);
    if (ident->val.id < 0)
    {
        ident->val.id = addglob(type->token, class, name, NULL, argc, local, root);
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
            typemod(t);
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
            typemod(t);
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
