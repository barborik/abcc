#ifndef __DEFS_
#define __DEFS_

/*
64-bit register | Lower 32 bits | Lower 16 bits | Lower 8 bits
==============================================================
rax             | eax           | ax            | al
rbx             | ebx           | bx            | bl
rcx             | ecx           | cx            | cl
rdx             | edx           | dx            | dl
rsi             | esi           | si            | sil
rdi             | edi           | di            | dil
rbp             | ebp           | bp            | bpl
rsp             | esp           | sp            | spl
r8              | r8d           | r8w           | r8b
r9              | r9d           | r9w           | r9b
r10             | r10d          | r10w          | r10b
r11             | r11d          | r11w          | r11b
r12             | r12d          | r12w          | r12b
r13             | r13d          | r13w          | r13b
r14             | r14d          | r14w          | r14b
r15             | r15d          | r15w          | r15b
*/

/*
parameter type               | first | second | third | fourth | fifth | sixth | seventh | eight | ninth and higher
====================================================================================================================
UNIX    - float              | XMM0  | XMM1   | XMM2  | XMM3   | XMM4  | XMM5  | XMM6    | XMM7  | stack
UNIX    - integer or pointer | rdi   | rsi    | rdx   | rcx    | r8    | r9    | stack
WINDOWS - float              | XMM0  | XMM1   | XMM2  | XMM3   | stack
WINDOWS - integer or pointer | rcx   | rdx    | r8    | r9     | stack
*/

// register list
// https://en.wikibooks.org/wiki/X86_Assembly/X86_Architecture
static char *reglist[] = {"rax", "rbx", "rcx", "rdx"};

// operator precedence table
//                         +  -  *  /        LITINT
static int prec[] = {NULL, 1, 1, 2, 2, NULL, 0};

enum
{
    // operators
    T_OPSTART,  // start and end pseudo-tokens for comparing ranges, if token is between T_OPSTART and T_OPEND then it is an operator
    T_PLUS,     // +
    T_MINUS,    // -
    T_ASTERISK, // *
    T_FSLASH,   // /
    T_EQUALS,   // ==
    T_OPEND,

    // literals
    T_INTLIT, // integer literal

    // types
    T_TSTART,
    T_I8,  // signed 8bit integer  (char)
    T_I16, // signed 16bit integer (short)
    T_I32, // signed 32bit integer (int)
    T_I64, // signed 64bit integer (long)
    T_TEND,

    // block statements
    BLOCK_START,
    T_IF,    // if statement
    T_WHILE, // while loop
    BLOCK_END,

    // other
    T_ASSIGN,    // =
    T_SEMICOLON, // ;
    T_IDENT,     // identifier
    T_LVIDENT,   // left value ident
    T_RVIDENT,   // right value ident
    T_JOIN,      // token for holding asnodes together

    // TODO: sort these
    T_LPAR,   // (
    T_RPAR,   // )
    T_LBRACE, // {
    T_RBRACE, // }
};

#endif