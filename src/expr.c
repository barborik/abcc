#include "includes.h"

Node *explist(void)
{
    Tok *tok;
    Node *root = NULL, *right = NULL;

    while (1)
    {
        if (tokseq(1, LT_RPAR))
        {
            break;
        }

        right = binexp(0);
        root = mknode(ST_JOIN, root, NULL, right);

        next(&tok);
        back();

        if (tokseq(1, LT_RPAR))
        {
            break;
        }
        next(&tok);
    }

    return root;
}

Node *prefix(Node *leaf)
{
    Tok  *tok;
    Node *root, *left;

    next(&tok);
    switch (tok->token)
    {
    case LT_PLUS:
        break;
    case LT_MINUS:
        root = mknode(ST_NEG, leaf, NULL, NULL);
        break;
    case LT_AMP:
        root = mknode(ST_ADDR, leaf, NULL, NULL);
        break;
    case LT_ASTERISK:
        left = prefix(leaf);
        root = mknode(ST_DEREF, left, NULL, NULL);
        break;
    case LT_EXCL:
        root = mknode(ST_LOGNOT, leaf, NULL, NULL);
        break;
    case LT_TILDA:
        root = mknode(ST_BITNOT, leaf, NULL, NULL);
        break;
    case LT_DPLUS:
        root = leaf;
        if (1) // TODO: make this so that it functions with both symbols and literals
        {
            setrval(leaf, 0);
            root = mknode(ST_ADDR, leaf, NULL, NULL);
        }
        root = mknode(ST_INC, root, NULL, NULL);
        root = mknode(ST_DEREF, root, NULL, NULL);
        break;
    case LT_DMINUS:
        root = leaf;
        if (1) // TODO: make this so that it functions with both symbols and literals
        {
            setrval(leaf, 0);
            root = mknode(ST_ADDR, leaf, NULL, NULL);
        }
        root = mknode(ST_DEC, root, NULL, NULL);
        root = mknode(ST_DEREF, root, NULL, NULL);
        break;
    default:
        root = leaf;
        back();
        break;
    }

    return root;
}

Node *postfix(Node *leaf)
{
    Type type_;

    Tok  *tok, offs;
    Node *root, *left, *right, *tmp;

    next(&tok);
    switch (tok->token)
    {
    case LT_LPAR:
        if (sym && sym->class != C_LOCL)
        {
            setrval(leaf, 0);
            leaf = mknode(ST_ADDR, leaf, NULL, NULL);
        }
        type_ = type;
        root = mknode(ST_CALL, leaf, explist(), NULL);
        type = type_;
        next(&tok);
        break;
    case LT_LSQBR:
        if (type.addr) type.addr--;
        offs.token = ST_INTLIT;
        offs.val.i = type2size(type);

        type_ = type;
        right = binexp(0);
        right = mknode(ST_MUL, mkleaf(&offs, NULL), NULL, right);
        type = type_;

        root = mknode(ST_ADD, leaf, NULL, right);
        root = mknode(ST_DEREF, root, NULL, NULL);

        next(&tok);
        break;
    case LT_DPLUS:
        right = leaf;
        if (sym)
        {
            right = mknode(ST_ADDR, mkleaf(&leaf->token, 0), NULL, NULL);
        }
        right = mknode(ST_INC, right, NULL, NULL);
        root = mknode(ST_LEFT, leaf, NULL, right);
        break;
    case LT_DMINUS:
        right = leaf;
        if (sym)
        {
            setrval(leaf, 0);
            right = mknode(ST_ADDR, mkleaf(&leaf->token, 0), NULL, NULL);
        }
        right = mknode(ST_DEC, right, NULL, NULL);
        root = mknode(ST_LEFT, leaf, NULL, right);
        break;
    default:
        root = leaf;
        back();
        break;
    }

    /*if (root != leaf)
    {
        root = postfix(root);
    }*/

    return root;
}

// build an unary expression
Node *unexp(void)
{
    char *str;
    Tok  *tok;
    Node *root;
    Node *leaf = mknode(NULL, NULL, NULL, NULL);
    Node *post = mknode(NULL, NULL, NULL, NULL);

    sym = NULL;
    setrval(leaf, 1);

    root = prefix(post);

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
    case ST_IDENT: /* FALLTGROUGH */
        sym = getsym(tok);
        type = sym->type;
        if (type.complex)
        {
            *leaf = *mknode(ST_ADDR, mkleaf(tok, 0), NULL, NULL);
            break;
        }
    default:
        *leaf = *mkleaf(tok, leaf->flags);
        break;
    }

    // *post = *postfix(leaf);
    copynode(post, postfix(leaf));

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
    bin2stx(op);
    while (prec[op->token] < ptp || ptp == 0 || (rassoc(op) && prec[op->token] == ptp))
    {
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
        bin2stx(op);
    }

    back();
    return left;
}
