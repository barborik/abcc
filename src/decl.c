#include "includes.h"

int typemod()
{
    Tok *t;
    int addr = 0;

    while (tokseq(1, LT_ASTERISK))
    {
        addr++;
        next(&t);
    }

    return addr;
}

int args(dlist_t *local)
{
    int argc = 0, addr;
    Tok *type, *ident, *t;

    next(&t); // (

    while (1)
    {
        if (tokseq(3, LT_DOT, LT_DOT, LT_DOT))
        {
            next(&t); // .
            next(&t); // .
            next(&t); // .
            argc = -1;
            break;
        }

        next(&type); // data type
        addr = typemod();

        if (type->token == LT_U0 || type->token == LT_I0)
        {
            break;
        }

        next(&ident); // ident

        Sym sym;
        sym.type.type = type->token;
        sym.type.addr = addr;
        sym.type.complex = 0;
        sym.class = C_LOCL;
        sym.size = 1;
        sym.name = *(char **)uniq->get[ident->val.i];
        sym.level = 1;
        sym.offs = 0;

        dl_add(local, &sym);
        argc++;

        if (tokseq(1, LT_RPAR))
        {
            break;
        }
        next(&t); // ,
    }

    next(&t); // )
    return argc;
}

Node *var_decl(int class)
{
    Sym *sym;
    Tok *type, *ident, *t;
    int size = 1, complex = 0, addr;
    char *name;

    next(&type); // data type

    // pointer modifiers
    addr = typemod();

    next(&ident); // identifier
    name = *(char **)uniq->get[ident->val.i];

    if (tokseq(1, LT_LSQBR))
    {
        next(&t); // [
        size = interpret(binexp(0));
        next(&t); // ]
        complex = 1;
    }

    next(&t); // semicolon

    // global variable
    if (class == C_GLOB || class == C_EXTN)
    {
        addglob(type->token, addr, complex, class, name, size, NULL, NULL, NULL);
        sym = getsym(ident);
        return NULL;
    }

    // local variable
    ident->token = ST_ALLOC;
    sym = addlocl(type->token, addr, complex, class, name, size);

    return mkleaf(ident, 0);
}

void func_decl(int class)
{
    char *name;
    int argc, addr;
    Tok *type, *ident, *t;
    Node *root;
    Sym *sym;
    dlist_t *local;

    next(&type);   // data type
    addr = typemod(type); // pointer modifiers

    next(&ident); // identifier
    name = *(char **)uniq->get[ident->val.i];

    local = malloc(sizeof(dlist_t));
    dl_init(local, sizeof(Sym));
    argc = args(local); // parse arguments

    // declaration
    if (tokseq(1, LT_SEMICOLON))
    {
        next(&t);
        addglob(type->token, addr, 0, class, name, NULL, argc, NULL, local);
        return;
    }

    // definition
    sym = findglob(name);
    if (!sym)
    {
        sym = addglob(type->token, addr, 0, class, name, NULL, argc, NULL, local);
    }

    func = sym;
    sym->root = mknode(ST_FUNC, block_stmt(), NULL, NULL);
}

void decl()
{
    Tok *t;
    while (next(&t))
    {
        level = 0;
        // printf("decl | %d %d\n", t->line, t->token);
        back();

        switch (t->token)
        {
        case LT_I0:
        case LT_I8:
        case LT_I16:
        case LT_I32:
        case LT_I64:
        case LT_U0:
        case LT_U8:
        case LT_U16:
        case LT_U32:
        case LT_U64:
            next(&t);
            typemod(t);
            if (tokseq(2, LT_IDENT, LT_LPAR))
            {
                back();
                func_decl(C_GLOB);
            }
            else
            {
                back();
                var_decl(C_GLOB);
            }
            break;
        case LT_EXTERN:
            next(&t);
            next(&t);
            typemod(t);
            if (tokseq(2, LT_IDENT, LT_LPAR))
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
