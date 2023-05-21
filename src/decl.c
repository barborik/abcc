#include "includes.h"

int typemod(void)
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

        if ((type->token == LT_U0 || type->token == LT_I0) && !addr)
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

Node *var_decl(int class, Type type_)
{
    char *name;
    int size = 1, complex = 0, addr;
    Sym *sym;
    Tok *type, *ident, *t;
    Node *root, *right;

    if (!type_.type)
    {
        next(&type);      // data type
        addr = typemod(); // pointer modifiers

        type_.type = type->token;
        type_.addr = addr;
    }

    next(&ident); // identifier
    // if (ident->token == LT_SEMICOLON) return NULL;

    ident->token = ST_IDENT;
    name = *(char **)uniq->get[ident->val.i];

    if (tokseq(1, LT_LSQBR))
    {
        next(&t); // [
        size = interpret(binexp(0));
        next(&t); // ]
        complex = 1;
    }

    type_.complex = complex;

    // global variable
    if (class == C_GLOB || class == C_EXTN)
    {
        next(&t); // semicolon
        addglob(type_, K_VAR, class, name, size, NULL, NULL, NULL);
        // sym = getsym(ident);
        return NULL;
    }

    sym = addlocl(type_, K_VAR, class, name, size);

    root = mknode(ST_ALLOC, mkleaf(ident, 0), NULL, NULL);

    if (tokseq(1, LT_ASSIGN))
    {
        next(&t);
        right = mknode(ST_ASSIGN, binexp(0), NULL, mkleaf(ident, 0));
        root = mknode(ST_JOIN, root, NULL, right);
    }

    next(&t); // semicolon or comma
    switch (t->token)
    {
    case LT_SEMICOLON:
        break;
    case LT_COMMA:
        root = mknode(ST_JOIN, root, NULL, var_decl(class, type_));
        break;
    }

    return root;
}

void func_decl(int class, Type type_)
{
    char *name;
    int argc, addr;
    Tok *type, *ident, *t;
    Node *root;
    Sym *sym;
    dlist_t *local;

    // next(&type);      // data type
    // addr = typemod(); // pointer modifiers

    next(&ident); // identifier
    name = *(char **)uniq->get[ident->val.i];

    local = malloc(sizeof(dlist_t));
    dl_init(local, sizeof(Sym));
    argc = args(local); // parse arguments

    // type_.type = type->token;
    // type_.addr = addr;
    type_.complex = 0;

    // declaration
    if (tokseq(1, LT_SEMICOLON))
    {
        next(&t);
        addglob(type_, K_FUNC, class, name, NULL, argc, NULL, local);
        return;
    }

    // definition
    sym = findglob(name);
    if (!sym)
    {
        sym = addglob(type_, K_FUNC, class, name, NULL, argc, NULL, local);
    }

    func = sym;
    sym->root = mknode(ST_FUNC, block_stmt(), NULL, NULL);
}

void tdef(void)
{
    Tok *tok;
    Type type;

    next(&tok); // typedef
    next(&tok); // type
    type.type = tok->token;
    type.addr = typemod();
    type.complex = 0;
    next(&tok); // ident

    addglob(type, K_TDEF, C_GLOB, *(char **)uniq->get[tok->val.i], NULL, NULL, NULL, NULL);

    next(&tok); // semi
}

void decl(void)
{
    Tok *t;
    Type type;

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
            type.type = t->token;
            type.addr = typemod();
            if (tokseq(2, LT_IDENT, LT_LPAR))
            {
                // back();
                func_decl(C_GLOB, type);
            }
            else
            {
                // back();
                var_decl(C_GLOB, type);
            }
            break;
        case LT_EXTERN:
            next(&t);
            next(&t);
            type.type = t->token;
            type.addr = typemod();
            if (tokseq(2, LT_IDENT, LT_LPAR))
            {
                // back();
                func_decl(C_EXTN, type);
            }
            else
            {
                // back();
                var_decl(C_EXTN, type);
            }
            break;
        case LT_TDEF:
            tdef();
            break;
        default:
            printf("DECL ERROR: line %d token %d\n", t->line, t->token);
            exit(1);
        }
    }

    return 0;
}
