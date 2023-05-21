#ifndef __DEFS_
#define __DEFS_

/*
https://en.wikibooks.org/wiki/X86_Assembly/X86_Architecture

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

typedef struct
{
    int nregs;

    char **reglist;
    int *regalloc;

    char **reglist64;
    char **reglist32;
    char **reglist16;
    char **reglist8;
} RegInfo;

/* 16-bit */
#define NREGS_STD16 6

static char *reglist16_std16[] = {"ax", "bx", "cx", "dx", "si", "di"};
static char *reglist8_std16[] = {"al", "bl", "cl", "dl", "sil", "dil"};

static int regalloc_std16[] = {0, 0, 0, 0, 0, 0};

static const RegInfo std16 = {
    .nregs = NREGS_STD16,
    .reglist = reglist16_std16,
    .regalloc = regalloc_std16,
    .reglist16 = reglist16_std16,
    .reglist8 = reglist8_std16,
};

/* 64-bit */
#define NREGS_STD64 14
#define NREGS_WIN64 4
#define NREGS_ELF64 6

static char *reglist64_std64[] = {"rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
static char *reglist32_std64[] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"};
static char *reglist16_std64[] = {"ax", "bx", "cx", "dx", "si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"};
static char *reglist8_std64[] = {"al", "bl", "cl", "dl", "sil", "dil", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b"};

static char *reglist64_win64[] = {"rcx", "rdx", "r8", "r9", /* FAKE --> */ "r10", "r11"};
static char *reglist32_win64[] = {"ecx", "edx", "r8d", "r9d", /* FAKE --> */ "r10d", "r11d"};
static char *reglist16_win64[] = {"cx", "dx", "r8w", "r9w", /* FAKE --> */ "r10w", "r11w"};
static char *reglist8_win64[] = {"cl", "dl", "r8b", "r9b", /* FAKE --> */ "r10b", "r11b"};

static char *reglist64_elf64[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
static char *reglist32_elf64[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
static char *reglist16_elf64[] = {"di", "si", "dx", "cx", "r8w", "r9w"};
static char *reglist8_elf64[] = {"dil", "sil", "dl", "cl", "r8b", "r9b"};

static int regalloc_std64[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int regalloc_win64[] = {0, 0, 0, 0};
static int regalloc_elf64[] = {0, 0, 0, 0, 0, 0};

static const RegInfo std64 = {
    .nregs = NREGS_STD64,
    .reglist = reglist64_std64,
    .regalloc = regalloc_std64,
    .reglist64 = reglist64_std64,
    .reglist32 = reglist32_std64,
    .reglist16 = reglist16_std64,
    .reglist8 = reglist8_std64,
};

static const RegInfo elf64 = {
    .nregs = NREGS_ELF64,
    .reglist = reglist64_elf64,
    .regalloc = regalloc_elf64,
    .reglist64 = reglist64_elf64,
    .reglist32 = reglist32_elf64,
    .reglist16 = reglist16_elf64,
    .reglist8 = reglist8_elf64,
};

static const RegInfo win64 = {
    .nregs = NREGS_STD64,
    .reglist = reglist64_win64,
    .regalloc = regalloc_win64,
    .reglist64 = reglist64_win64,
    .reglist32 = reglist32_win64,
    .reglist16 = reglist16_win64,
    .reglist8 = reglist8_win64,
};

#if defined(_WIN32)
static int ALIGN = 32;
static RegInfo arg64 = win64;
#else
static int ALIGN = 16;
static RegInfo arg64 = elf64;
#endif

/* DEFINES */
#define NULLREG -1

/* TYPEDEFS */
typedef struct tok Tok;
typedef struct sym Sym;
typedef struct type Type;
typedef struct node Node;

/* EXTERNS */
extern int mode;
extern RegInfo *reginfo;

extern Sym *sym;
extern Sym *func;
extern Type type;

extern char *src_n;
extern char *out_n;

extern FILE *src_f;
extern FILE *out_f;

/* TARGET MODES */
enum
{
    M_16,
    M_64,
};

/* OPERATOR PRECEDENCE TABLE */
static int prec[] = {
    NULL,             // OP_START
    3, 3, 3,          // * / %
    4, 4,             // + -
    5, 5,             // << >>
    6, 6, 6, 6, 6, 6, // < <= > >= == !=
    7, 7, 7,          // & ^ |
    8, 8,             // && ||
    9,                // =
};

/* LEXICAL TOKENS */
enum
{
    /*
    start and end pseudo-tokens for comparing ranges,
    if token is between T_OPSTART and T_OPEND then it is an operator
    */
    LT_OP_START,
    LT_PLUS,     // +
    LT_MINUS,    // -
    LT_DPLUS,    // ++
    LT_DMINUS,   // --
    LT_ASTERISK, // *
    LT_FSLASH,   // /
    LT_PERCENT,  // %
    LT_LSHIFT,   // <<
    LT_RSHIFT,   // >>
    LT_EQ,       // ==
    LT_NE,       // !=
    LT_GT,       // >
    LT_LT,       // <
    LT_GE,       // >=
    LT_LE,       // <=
    LT_AMP,      // &
    LT_DAMP,     // &&
    LT_PIPE,     // |
    LT_DPIPE,    // ||
    LT_EXCL,     // !
    LT_TILDA,    // ~
    LT_ASSIGN,   // =
    LT_CARET,    // ^
    LT_SIZEOF,   // sizeof()
    LT_OP_END,

    LT_LIT_START,
    LT_INTLIT,   // integer literal
    LT_FLOATLIT, // float literal
    LT_STRLIT,   // string literal
    LT_CHARLIT,  // character literal
    LT_LIT_END,

    LT_TYPE_START,
    LT_I0, // void
    LT_U0, // void

    LT_I8,  // signed 8bit integer  (char)
    LT_I16, // signed 16bit integer (short)
    LT_I32, // signed 32bit integer (int)
    LT_I64, // signed 64bit integer (long)

    LT_U8,  // unsigned 8bit integer  (unsigned char)
    LT_U16, // unsigned 16bit integer (unsigned short)
    LT_U32, // unsigned 32bit integer (unsigned int)
    LT_U64, // unsigned 64bit integer (unsigned long)
    LT_TYPE_END,

    LT_BLOCK_START,
    LT_IF,    // if
    LT_ELSE,  // else
    LT_WHILE, // while
    LT_FOR,   // for
    LT_ASM,   // inline assembly
    LT_BLOCK_END,

    LT_IDENT,    // identifier
    LT_EXTERN,   // extern
    LT_RETURN,   // return
    LT_BREAK,    // break
    LT_CONTINUE, // continue
    LT_GOTO,     // goto
    LT_TDEF,     // typedef

    LT_COLON,     // :
    LT_SEMICOLON, // ;
    LT_LPAR,      // (
    LT_RPAR,      // )
    LT_LBRACE,    // {
    LT_RBRACE,    // }
    LT_LSQBR,     // [
    LT_RSQBR,     // ]
    LT_COMMA,     // ,
    LT_DOT,       // .
};

/* SYNTAX TOKENS */
enum
{
    /* === OPERATIONS === */
    ST_OP_START,
    /* BINARY */
    ST_MUL, // *
    ST_DIV, // /
    ST_MOD, // %

    ST_ADD, // +
    ST_SUB, // -

    ST_LSHIFT, // <<
    ST_RSHIFT, // >>

    ST_EQ, // ==
    ST_NE, // !=
    ST_GT, // >
    ST_LT, // <
    ST_GE, // >=
    ST_LE, // <=

    ST_BITAND, // &
    ST_BITXOR, // ^
    ST_BITOR,  // |

    ST_LOGAND, // &&
    ST_LOGOR,  // ||

    ST_ASSIGN, // =

    /* UNARY */
    ST_SIZEOF, // sizeof()
    ST_LOGNOT, // !
    ST_BITNOT, // ~
    ST_NEG,    // -
    ST_ADDR,   // &
    ST_DEREF,  // *
    ST_CALL,   // function call
    ST_INC,    // ++
    ST_DEC,    // --
    ST_OP_END,

    /* === LITERALS === */
    ST_LIT_START,
    ST_INTLIT,   // integer literal
    ST_FLOATLIT, // float literal
    ST_STRLIT,   // string literal
    ST_CHARLIT,  // character literal
    ST_LIT_END,

    /* IDENTIFIER */
    ST_IDENT, // identifier

    /* === BLOCK STATEMENTS === */
    ST_BLOCK_START,
    ST_IF,    // if statement
    ST_WHILE, // while loop
    ST_FOR,   // for loop
    ST_ASM,   // inline assembly
    ST_BLOCK_END,

    /* OTHER */
    ST_RETURN,    // return
    ST_BREAK,     // break
    ST_CONTINUE,  // continue
    ST_GOTO,      // goto
    ST_LABEL,     // label
    ST_SEMICOLON, // ;
    ST_JOIN,      // token for holding nodes together
    ST_LEFT,      // return left register
    ST_RIGHT,     // return right register
    ST_FUNC,      // function (or procedure)
    ST_ALLOC,     // stack allocation (local variable declaration)
};

#endif