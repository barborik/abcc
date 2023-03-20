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
#define NARGS_WIN64 4
#define NARGS_ELF64 6

static char *reglist64_win64[] = {"rcx", "rdx", "r8", "r9"};
static char *reglist32_win64[] = {"ecx", "edx", "r8d", "r9d"};
static char *reglist16_win64[] = {"cx", "dx", "r8w", "r9w"};
static char *reglist8_win64[] = {"cl", "dl", "r8b", "r9b"};

static char *reglist64_elf64[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
static char *reglist32_elf64[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
static char *reglist16_elf64[] = {"di", "si", "rdx", "cx", "r8w", "r9w"};
static char *reglist8_elf64[] = {"dil", "sil", "dl", "cl", "r8b", "r9b"};

static int regalloc_win64[] = {0, 0, 0, 0};
static int regalloc_elf64[] = {0, 0, 0, 0, 0, 0};

static char **reglist64;
static char **reglist32;
static char **reglist16;
static char **reglist8;

static char **reglist;
static int *regalloc;

// lexical tokens
enum
{
    /*
    start and end pseudo-tokens for comparing ranges,
    if token is between T_OPSTART and T_OPEND then it is an operator
    */
    T_OP_START,
    T_PLUS,     // +
    T_MINUS,    // -
    T_ASTERISK, // *
    T_FSLASH,   // /
    T_EQ,       // ==
    T_NE,       // !=
    T_GT,       // >
    T_LT,       // <
    T_GE,       // >=
    T_LE,       // <=
    T_AMP,      // &
    T_DAMP,     // &&
    T_PIPE,     // |
    T_DPIPE,    // ||
    T_EXCL,     // !
    T_TILDA,    // ~
    T_ASSIGN,   // =
    T_OP_END,

    T_LIT_START,
    T_INTLIT, // integer literal
    T_LIT_END,

    T_TYPE_START,
    // value types
    T_TYPE_VAL_START,
    T_I0, // void
    T_U0, // void

    T_I8,  // signed 8bit integer  (char)
    T_I16, // signed 16bit integer (short)
    T_I32, // signed 32bit integer (int)
    T_I64, // signed 64bit integer (long)

    T_U8,  // unsigned 8bit integer  (unsigned char)
    T_U16, // unsigned 16bit integer (unsigned short)
    T_U32, // unsigned 32bit integer (unsigned int)
    T_U64, // unsigned 64bit integer (unsigned long)
    T_TYPE_VAL_END,

    // pointers
    T_TYPE_PTR_START,
    T_I0PTR,
    T_U0PTR,

    T_I8PTR,
    T_I16PTR,
    T_I32PTR,
    T_I64PTR,

    T_U8PTR,
    T_U16PTR,
    T_U32PTR,
    T_U64PTR,
    T_TYPE_PTR_END,

    // double pointers
    T_TYPE_DPTR_START,
    T_I0PTRPTR,
    T_U0PTRPTR,

    T_I8PTRPTR,
    T_I16PTRPTR,
    T_I32PTRPTR,
    T_I64PTRPTR,

    T_U8PTRPTR,
    T_U16PTRPTR,
    T_U32PTRPTR,
    T_U64PTRPTR,
    T_TYPE_DPTR_END,
    T_TYPE_END,

    T_BLOCK_START,
    T_IF,    // if statement
    T_WHILE, // while loop
    T_BLOCK_END,

    // TODO: sort these
    T_EXTERN,    // extern
    T_RETURN,    // return
    T_SEMICOLON, // ;
    T_IDENT,     // identifier
    T_LPAR,      // (
    T_RPAR,      // )
    T_LBRACE,    // {
    T_RBRACE,    // }
    T_LSQBR,     // [
    T_RSQBR,     // ]
    T_COMMA,     // ,
    T_DOT,       // .
};

// operator precedence table
static int prec[] = {
    NULL, // OP_START
    /*
    +  -  *  /  == != >  <  >= <= && || &  |  =  !  ~ */
    1, 1, 2, 2, 4, 4, 3, 3, 3, 3, 1, 1, 1, 1, 0, 1, 1,
    1,    // REF &
    1,    // DEREF *
    NULL, // OP_END
    NULL, // LIT_START
    0     // INTLIT
};

// sytax tokens
enum
{
    /* === OPERATIONS === */
    ST_OP_START,
    // binary
    ST_ADD,    // +
    ST_SUB,    // -
    ST_MUL,    // *
    ST_DIV,    // /
    ST_EQ,     // ==
    ST_NE,     // !=
    ST_GT,     // >
    ST_LT,     // <
    ST_GE,     // >=
    ST_LE,     // <=
    ST_LOGAND, // &&
    ST_LOGOR,  // ||
    ST_BITAND, // &
    ST_BITOR,  // |
    ST_ASSIGN, // =

    // unary
    ST_LOGNOT, // !
    ST_BITNOT, // ~
    ST_ADDR,   // &
    ST_DEREF,  // *
    ST_OP_END,

    /* === LITERALS === */
    ST_LIT_START,
    ST_INTLIT,
    ST_LIT_END,

    /* === BLOCK STATEMENTS === */
    ST_BLOCK_START,
    ST_IF,
    ST_WHILE,
    ST_BLOCK_END,

    // other
    ST_RETURN,    // return
    ST_SEMICOLON, // ;
    ST_IDENT,     // identifier
    ST_JOIN,      // token for holding asnodes together
    ST_FUNC,      // function (or procedure)
    ST_CALL,      // function call
};

#endif