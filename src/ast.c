#include "includes.h"

// make (construct) an asnode
Node *mknode(int token, Node *left, Node *mid, Node *right)
{
    Node *node = malloc(sizeof(Node));

    node->flags = 0b00000001;
    node->token.token = token;
    node->left = left;
    node->mid = mid;
    node->right = right;

    // setrval(node, 1);

    return node;
}

Node *mkleaf(Tok *token, int flags)
{
    Node *node = malloc(sizeof(Node));

    node->flags = flags;
    node->token = *token;
    node->left = NULL;
    node->mid = NULL;
    node->right = NULL;

    // setrval(node, 1);

    return node;
}

// checks if a given token is an operator token
int isop(int token)
{
    if (token > LT_OP_START && token < LT_OP_END ||
        token > ST_OP_START && token < ST_OP_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a type token
int istype(int token)
{
    if (token > LT_TYPE_START && token < LT_TYPE_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a literal token
int isliteral(int token)
{
    if (token > LT_LIT_START && token < LT_LIT_END ||
        token > ST_LIT_START && token < ST_LIT_END)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is an identifier token
int isident(int token)
{
    if (token == LT_IDENT || token == ST_IDENT)
    {
        return 1;
    }
    return 0;
}

// checks if a given token is a block token
int isblock(int token)
{
    if (token > LT_BLOCK_START && token < LT_BLOCK_END ||
        token > ST_BLOCK_START && token < ST_BLOCK_END)
    {
        return 1;
    }
    return 0;
}

int rassoc(int token)
{
    if (token == LT_ASSIGN || token == ST_ASSIGN)
    {
        return 1;
    }
    return 0;
}

int getrval(Node *node)
{
    return node->flags & 0b00000001;
}

void setrval(Node *node, int val)
{
    (val) ? (node->flags |= 0b00000001) : (node->flags &= 0b11111110);
}

Node *explist(void)
{
    Tok  *tok;
    Node *root = NULL, *right = NULL;

    while (1)
    {
        if (tokseq(1, LT_RPAR))
        {
            break;
        }

        right = binexp(0);
        root = mknode(ST_JOIN, root, NULL, right);

        if (tokseq(1, LT_RPAR))
        {
            break;
        }
        next(&tok);
    }

    return root;
}

Node *prefix(Node* leaf)
{
    Tok  *tok;
    Node *root = leaf, *left;

    next(&tok);
    switch (tok->token)
    {
    case LT_AMP:
        root = mknode(ST_ADDR, leaf, NULL, NULL);
        break;
    case LT_ASTERISK:
        left = prefix(leaf);
        root = mknode(ST_DEREF, left, NULL, NULL);
        break;
    case LT_DPLUS:
        setrval(leaf, 0);
        left = mknode(ST_ADDR, leaf, NULL, NULL);
        root = mknode(ST_INC, left, NULL, NULL);
        break;
    case LT_DMINUS:
        setrval(leaf, 0);
        left = mknode(ST_ADDR, leaf, NULL, NULL);
        root = mknode(ST_DEC, left, NULL, NULL);
        break;
    default:
        back();
        break;
    }

    return root;
}

Node *postfix(Node *leaf)
{
    Tok  *tok, offs;
    Node *root, *left, *right;

    next(&tok);
    switch (tok->token)
    {
    case LT_LPAR:
        root = mknode(ST_CALL, leaf, explist(), NULL);
        next(&tok);
        break;
    case LT_LSQBR:
        offs.token = ST_INTLIT;
        offs.val.i = type2size(type);

        right = binexp(0);
        right = mknode(ST_MUL, mkleaf(&offs, 0), NULL, right);

        root = mknode(ST_ADD, leaf, NULL, right);
        root = mknode(ST_DEREF, root, NULL, NULL);
        setrval(root, 1);
        
        next(&tok);
        break;
    case LT_DPLUS:
        left = mknode(ST_ADDR, leaf, NULL, NULL);
        root = mknode(ST_INC, left, NULL, NULL);
        break;
    case LT_DMINUS:
        left = mknode(ST_ADDR, leaf, NULL, NULL);
        root = mknode(ST_DEC, left, NULL, NULL);
        break;
    default:
        root = leaf;
        back();
        break;
    }

    return root;
}

// build an unary expression
Node *unexp(void)
{
    char *str;
    Tok  *tok;
    Sym  *sym;
    Node *root, *leaf = mknode(NULL, NULL, NULL, NULL);

    setrval(leaf, 1);

    root = prefix(leaf);

    next(&tok);
    un2stx(tok);
    memset(&type, 0, sizeof(Type));

    switch (tok->token)
    {
    case ST_STRLIT:
        str = *(char **)uniq->get[tok->val.i];
        sym = findglob(str);
        if (!sym)
        {
            addglob(LT_I8, 1, 1, C_DATA, str, strlen(str), NULL, NULL, NULL);
        }
        return mkleaf(tok, 0);
    case LT_LPAR:
        *leaf = *binexp(0);
        next(&tok);
        break;
    case ST_IDENT:
        type = getsym(tok)->type;
    default:
        *leaf = *mkleaf(tok, leaf->flags);
        break;
    }
    
    if (type.complex)
    {
        setrval(leaf, 0);
        leaf = mknode(ST_ADDR, leaf, NULL, NULL);
    }
    
    root = postfix(root);

    return root;
}

// build a binary expression with pratt parsing
Node *binexp(int ptp)
{
    Tok  *op, *tok;
    Node *left, *right, *tmp;

    // left operand
    // next(&token_l);
    // bin2stx(token_l);

    left = unexp();
    tok = &left->token;

    if (tok->token == ST_STRLIT)
    {
        return left;
    }

    /*if (left->left && tok->token == ST_ADDR && left->left->token.token == ST_IDENT)
    {
        setrval(left->left, 0);
    }*/

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

        left = mknode(op->token, left, NULL, right);

        if (!next(&op) || !isop(op->token))
        {
            back();
            return left;
        }
    }

    back();
    return left;
}

int interpret(Node *root)
{
    int left, right;
    Tok *t = &root->token;

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
    case ST_ADD:
        return left + right;
    case ST_SUB:
        return left - right;
    case ST_MUL:
        return left * right;
    case ST_DIV:
        return left / right;
    case ST_INTLIT:
        return t->val.i;
    case ST_CHARLIT:
        return t->val.i;
    }

    printf("ERROR: line %d token %d\n", t->line, t->token);
    exit(1);
}
