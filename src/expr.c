#include "includes.h"

int recur = 0, deref;

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
    case LT_AMP:
        root = mknode(ST_ADDR, leaf, NULL, NULL);
        break;
    case LT_ASTERISK:
        left = prefix(leaf);
        root = mknode(ST_DEREF, left, NULL, NULL);
        break;
    case LT_DPLUS:
        //setrval(leaf, 0);
        //root = mknode(ST_ADDR, leaf, NULL, NULL);
        root = mknode(ST_INC, leaf, NULL, NULL);
        //root = mknode(ST_DEREF, root, NULL, NULL);
        break;
    case LT_DMINUS:
        //setrval(leaf, 0);
        //root = mknode(ST_ADDR, leaf, NULL, NULL);
        root = mknode(ST_DEC, leaf, NULL, NULL);
        //root = mknode(ST_DEREF, root, NULL, NULL);
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
    Tok  *tok, offs;
    Node *root, *left, *right, *tmp;

    next(&tok);
    switch (tok->token)
    {
    case LT_LPAR:
        recur = 0;
        root = mknode(ST_CALL, leaf, explist(), NULL);
        next(&tok);
        break;
    case LT_LSQBR:
        left = mknode(ST_DEREF, leaf, NULL, NULL);

        offs.token = ST_INTLIT;
        offs.val.i = type2size(type);

        right = binexp(0);
        right = mknode(ST_MUL, mkleaf(&offs, 0), NULL, right);

        root = mknode(ST_ADD, left, NULL, right);
        //root = mknode(ST_DEREF, root, NULL, NULL);

        //setrval(root, 1);
        next(&tok);
        break;
    case LT_DPLUS:
        right = mknode(ST_INC, leaf, NULL, NULL);
        left = mknode(ST_DEREF, leaf, NULL, NULL);
        root = mknode(ST_LEFT, left, NULL, right);
        break;
    case LT_DMINUS:
        right = mknode(ST_DEC, leaf, NULL, NULL);
        left = mknode(ST_DEREF, leaf, NULL, NULL);
        root = mknode(ST_LEFT, left, NULL, right);
        break;
    default:
        root = leaf;
        back();
        break;
    }

    if (root != leaf)
    {
        root = postfix(root);
    }

    return root;
}

// build an unary expression
Node *unexp(void)
{
    int  deref_ = 0;
    char *str;
    Tok  *tok;
    Sym  *sym;
    Node *root;
    Node *leaf = mknode(NULL, NULL, NULL, NULL);
    Node *post = mknode(NULL, NULL, NULL, NULL);

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
        if (deref) deref_ = 1;
        break;
    case ST_IDENT:
        deref_ = 1;
        type = getsym(tok)->type;
        *leaf = *mknode(ST_ADDR, mkleaf(tok, 0), NULL, NULL);
        break;
    default:
        *leaf = *mkleaf(tok, leaf->flags);
        break;
    }

    *post = *postfix(leaf);

    /*if (deref && root->token.token != ST_CALL)
    {
        root = mknode(ST_DEREF, root, NULL, NULL);
    }*/
    if (root->token.token == ST_LEFT)
    {
        deref_ = 0;
    }

    /*if (root->token.token == ST_INC || root->token.token == ST_DEC)
    {
        deref_ = 1;
    }*/

    deref = deref_;

    return root;
}

// build a binary expression with pratt parsing
Node *binexp(int ptp)
{
    Tok  *op, *tok;
    Node *left, *right, *tmp;

    recur++;

    // left operand
    // next(&token_l);
    // bin2stx(token_l);

    left = unexp();
    tok = &left->token;

    if (tok->token == ST_STRLIT)
    {
        recur--;
        return left;
    }

    /*if (left->left && tok->token == ST_ADDR && left->left->token.token == ST_IDENT)
    {
        setrval(left->left, 0);
    }*/

    // operator (if available)
    if (!next(&op) || !isop(op->token))
    {
        recur--;
        if (!recur && deref && left->token.token != ST_CALL)
        {
            deref = 0;
            left = mknode(ST_DEREF, left, NULL, NULL);
        }
        back();
        return left;
    }

    if (deref && left->token.token != ST_CALL)
    {
        deref = 0;
        left = mknode(ST_DEREF, left, NULL, NULL);
    }

    recur--;

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

            if (deref && left->token.token != ST_CALL)
            {
                deref = 0;
                left = mknode(ST_DEREF, left, NULL, NULL);
            }
        }
        else
        {
            setrval(left, 1);
            setrval(right, 1);

            if (deref && right->token.token != ST_CALL)
            {
                deref = 0;
                right = mknode(ST_DEREF, right, NULL, NULL);
            }
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
