#include "includes.h"

// make (construct) an asnode
asnode_t *mknode(token_t *token, asnode_t *left, asnode_t *mid, asnode_t *right)
{
    asnode_t *node = malloc(sizeof(asnode_t));

    node->token = token;
    node->left = left;
    node->mid = mid;
    node->right = right;
    node->flags = 0;

    return node;
}

// checks if a given token is an operator token
int isop(int token)
{
    if (token > T_OP_START && token < T_OP_END ||
        token > ST_OP_START && token < ST_OP_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a type token
int istype(int token)
{
    if (token > T_TYPE_START && token < T_TYPE_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a value type token
int isvaltype(int token)
{
    if (token > T_TYPE_VAL_START && token < T_TYPE_VAL_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a pointer token
int isptr(int token)
{
    if (token > T_TYPE_PTR_START && token < T_TYPE_PTR_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a double pointer token
int isdptr(int token)
{
    if (token > T_TYPE_DPTR_START && token < T_TYPE_DPTR_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a literal token
int isliteral(int token)
{
    if (token > T_LIT_START && token < T_LIT_END ||
        token > ST_LIT_START && token < ST_LIT_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is an identifier token
int isident(int token)
{
    if (token == T_IDENT || token == ST_IDENT)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a block token
int isblock(int token)
{
    if (token > T_BLOCK_START && token < T_BLOCK_END ||
        token > ST_BLOCK_START && token < ST_BLOCK_END)
    {
        return 1;
    }
    return 0;
}

int rassoc(int token)
{
    if (token == T_ASSIGN || token == ST_ASSIGN)
    {
        return 1;
    }
    return 0;
}

int getrval(asnode_t *node)
{
    return node->flags & 0b00000001;
}

void setrval(asnode_t *node, int val)
{
    (val) ? (node->flags |= 0b00000001) : (node->flags &= 0b11111110);
}

asnode_t *arrindex()
{
    sym_t *sym;
    asnode_t *root, *left, *right;
    token_t *ident, *addr, *lbr, *rbr, *mul, *offs;

    next(&ident); // ident

    if (findlocl(*(char **)uniq->get[ident->val.id]) < 0)
    {
        ident->class = C_GLOB;
        ident->val.id = findglob(*(char **)uniq->get[ident->val.id]);

        sym = glob->get[ident->val.id];
    }
    else
    {
        ident->class = C_LOCL;
        ident->val.id = findlocl(*(char **)uniq->get[ident->val.id]);

        sym = func->local->get[ident->val.id];
    }

    ident->token = ST_IDENT;

    if (sym->prim)
    {
        left = mknode(ident, NULL, NULL, NULL);
        setrval(left, 1);
    }
    else
    {
        addr = malloc(sizeof(token_t));
        addr->token = ST_ADDR;
        left = mknode(addr, mknode(ident, NULL, NULL, NULL), NULL, NULL);
    }

    // parse the square brackets
    next(&lbr); // [
    right = binexp(0);
    next(&rbr); // ]

    // calculate index offset
    offs = malloc(sizeof(token_t));
    offs->token = ST_INTLIT;
    offs->val.i = type2size(sym->type);

    mul = malloc(sizeof(token_t));
    mul->token = ST_MUL;
    right = mknode(mul, right, NULL, mknode(offs, NULL, NULL, NULL));

    lbr->token = ST_ADD;
    root = mknode(lbr, left, NULL, right);

    // dereference
    rbr->token = ST_DEREF;
    root = mknode(rbr, root, NULL, NULL);
    setrval(root, 1);

    return root;
}

// build an unary expression
asnode_t *unexp()
{
    token_t *t;
    asnode_t *root;
    sym_t *sym = NULL;

    next(&t);

    switch (t->token)
    {
    case T_STRLIT:
        un2stx(t);
        char *str = *(char **)uniq->get[t->val.id];
        t->val.id = findglob(str);
        if (t->val.id < 0)
        {
            t->val.id = addglob(T_STRLIT, C_DATA, str, strlen(str), NULL, NULL, NULL);
        }
        return mknode(t, NULL, NULL, NULL);
    case T_IDENT:
        back();
        if (tokseq(2, T_IDENT, T_LPAR))  return func_call(0);
        if (tokseq(2, T_IDENT, T_LSQBR)) return arrindex();
        next(&t);
        break;
    case T_LPAR:
        root = binexp(0);
        setrval(root, 1);
        next(&t); // )
        return root;
    }

    un2stx(t);
    root = mknode(t, NULL, NULL, NULL);
    setrval(root, 1);

    if (isident(t->token))
    {
        if (t->class == C_GLOB) sym = glob->get[t->val.id];
        if (t->class == C_LOCL) sym = func->local->get[t->val.id];

        if (!sym->prim)
        {
            token_t *addr = malloc(sizeof(token_t));
            addr->token = ST_ADDR;
            root = mknode(addr, root, NULL, NULL);
            setrval(root, 1);
        }
    }

    if (!isliteral(t->token) && !isident(t->token))
    {
        root->left = unexp();
    }

    return root;
}

// build a binary expression with pratt parsing
asnode_t *binexp(int ptp)
{
    token_t *op, *token_l;
    asnode_t *left, *right, *tmp;

    // left operand
    // next(&token_l);
    // bin2stx(token_l);
    left = unexp();

    if (left->token->token == T_STRLIT)
    {
        return left;
    }

    if (left->left && left->token->token == ST_ADDR && left->left->token->token == ST_IDENT)
    {
        setrval(left->left, 0);
    }

    // operator (if available)
    if (!next(&op) || !isop(op->token))
    {
        back();
        return left;
    }

    // right operand + recursively get the rest
    while (prec[op->token] > ptp || (rassoc(op) && prec[op->token] == ptp))
    {
        bin2stx(op);
        right = binexp(prec[op->token]);

        if (op->token == ST_ASSIGN)
        {
            setrval(left, 0);
            setrval(right, 1);
            tmp = left;
            left = right;
            right = tmp;
        }
        else
        {
            setrval(left, 1);
            setrval(right, 1);
        }

        left = mknode(op, left, NULL, right);

        if (!next(&op) || !isop(op->token))
        {
            back();
            return left;
        }
    }

    back();
    return left;
}

int interpret(asnode_t *root)
{
    int left, right;
    token_t *t = root->token;

    if (root->left)
    {
        left = interpret(root->left);
    }
    if (root->right)
    {
        right = interpret(root->right);
    }

    switch (t->token)
    {
    case ST_ADD: return left + right;
    case ST_SUB: return left - right;
    case ST_MUL: return left * right;
    case ST_DIV: return left / right;
    case ST_INTLIT: return t->val.i;
    case ST_CHARLIT: return t->val.c;
    }
    
    printf("ERROR: line %d token %d\n", t->line, t->token);
    exit(1);  
}
