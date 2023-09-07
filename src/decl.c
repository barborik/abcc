#include "includes.h"

void typemod(Type *type)
{
    Tok *t;

    type->size = 1;
    next(&t);
    switch (t->token)
    {
    case LT_ASTERISK:
        while (tokseq(1, LT_ASTERISK))
        {
            type->addr++;
            next(NULL);
        }
        break;
    case LT_LSQBR:
        type->size = interpret(binexp(0));
        next(NULL);
        break;
    default:
        back();
    }
}

int args(dlist_t *local)
{
    Tok *t;
    char *name;
    int argc = 0;

    next(NULL); // (

    while (1)
    {
        if (tokseq(3, LT_DOT, LT_DOT, LT_DOT))
        {
            next(NULL); // .
            next(NULL); // .
            next(NULL); // .
            argc = -1;
            break;
        }

        next(&t);

        if (t->token == LT_U0 || t->token == LT_I0)
        {
            break;
        }

        name = *(char **)uniq->get[t->val.i];

        next(NULL);
        next(&t);
        type.type = t->token;
        typemod(&type);

        Sym sym;
        sym.type = type;
        sym.class = C_LOCL;
        sym.name = name;
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

    next(NULL); // )
    return argc;

    /*int argc = 0, addr;
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
    return argc;*/
}

Node *var_decl(int class)
{
    char *name;
    Tok *ident, *t;
    Node *root = NULL;

    // identifier
    next(&ident);
    ident->token = ST_IDENT;
    name = *(char **)uniq->get[ident->val.i];

    // , or :
    next(&t);
    if (t->token == LT_COMMA)
    {
        root = var_decl(class);

        if (class == C_GLOB || class == C_EXTN)
        {
            addglob(type, K_VAR, class, name, NULL, NULL, NULL);
            return NULL;
        }

        addlocl(type, K_VAR, class, name);
        root = mknode(ST_JOIN, root, NULL, mknode(ST_ALLOC, mkleaf(ident, 0), NULL, NULL));

        return root;
    }

    // type
    next(&t);
    type.type = t->token;
    typemod(&type);

    // ;
    next(NULL);

    if (class == C_GLOB || class == C_EXTN)
    {
        addglob(type, K_VAR, class, name, NULL, NULL, NULL);
        return NULL;
    }

    addlocl(type, K_VAR, class, name);
    root = mknode(ST_ALLOC, mkleaf(ident, 0), NULL, NULL);

    return root;

    /*char *name;
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

    return root;*/
}

void func_decl(int class)
{
    Sym *sym;
    char *name;
    Tok *ident, *t;
    Node *root = NULL;

    int argc;
    dlist_t *local;

    // identifier
    next(&ident);
    name = *(char **)uniq->get[ident->val.i];

    // arguments
    local = malloc(sizeof(dlist_t));
    dl_init(local, sizeof(Sym));
    argc = args(local);

    // :
    next(NULL);

    // type
    next(&t);
    type.type = t->token;
    typemod(&type);

    // declaration
    if (tokseq(1, LT_SEMICOLON))
    {
        next(NULL);
        addglob(type, K_FUNC, class, name, argc, NULL, local);
        return;
    }

    // definition
    sym = findglob(name);
    if (!sym)
    {
        sym = addglob(type, K_FUNC, class, name, argc, NULL, local);
    }

    func = sym;
    sym->root = mknode(ST_FUNC, block_stmt(), NULL, NULL);

    /*char *name;
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
    sym->root = mknode(ST_FUNC, block_stmt(), NULL, NULL);*/
}

void enum_(void)
{
    Tok *t;
    Sym *s;
    int i = 1;
    char *name;

    next(NULL); // enum
    next(NULL); // {

    while (next(&t))
    {
        if (t->token == LT_RBRACE)
        {
            break;
        }

        if (tokseq(1, LT_COMMA))
        {
            next(NULL);
        }

        name = *(char **)uniq->get[t->val.i];
        s = addglob(type, K_ENUM, C_GLOB, name, NULL, NULL, NULL);
        s->eval = i;

        i++;
    }
}

void tdef(void)
{
    /*Tok *tok;
    Type type;

    next(&tok); // typedef
    next(&tok); // type
    type.type = tok->token;
    type.addr = typemod();
    type.complex = 0;
    next(&tok); // ident

    addglob(type, K_TDEF, C_GLOB, *(char **)uniq->get[tok->val.i], NULL, NULL, NULL, NULL);

    next(&tok); // semi*/
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
            /*next(&t);
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
            }*/
            break;
        case LT_IDENT:
            // back();
            if (tokseq(2, LT_IDENT, LT_LPAR))
            {
                func_decl(C_GLOB);
            }
            else
            {
                var_decl(C_GLOB);
            }
            break;
        // case LT_EXTERN:
        case LT_ATMARK:
            // next(&t);
            // next(&t);
            // type.type = t->token;
            // type.addr = typemod();
            next(NULL);
            if (tokseq(2, LT_IDENT, LT_LPAR))
            {
                // back();
                func_decl(C_EXTN);
            }
            else
            {
                // back();
                var_decl(C_EXTN);
            }
            break;
        case LT_TDEF:
            tdef();
            break;
        case LT_ENUM:
            enum_();
            break;
        default:
            printf("DECL ERROR: line %d token %d\n", t->line, t->token);
            exit(1);
        }
    }

    return 0;
}
