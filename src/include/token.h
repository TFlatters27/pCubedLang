#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
  TOKEN_ID = 1 << 0,         // 1 : identifier
  TOKEN_ASSIGNMENT = 1 << 1, // 2 : <-
  TOKEN_STRING = 1 << 2,     // 4 : string
  TOKEN_INT = 1 << 3,        // 8 : integer
  TOKEN_REAL = 1 << 4,       // 16 : real
  TOKEN_BOOL = 1 << 5,       // 32 : True | False
  TOKEN_CHAR = 1 << 6,       // 64 : character
  TOKEN_NEWLINE = 1 << 7,    // 128 : \n
  TOKEN_LPAREN = 1 << 8,     // 256 : (
  TOKEN_RPAREN = 1 << 9,     // 512 : )
  TOKEN_LBRACKET = 1 << 10,  // 1024 : [
  TOKEN_RBRACKET = 1 << 11,  // 2048 : ]
  TOKEN_LBRACE = 1 << 12,    // 4096 : {
  TOKEN_RBRACE = 1 << 13,    // 8192 : }
  TOKEN_ARITH_OP = 1 << 14,  // 16384 : +|-|*|/|^|DIV|MOD
  TOKEN_REL_OP = 1 << 15,    // 32768 : < | > | = | != | <= | >=
  TOKEN_BOOL_OP = 1 << 16,   // 65536 : AND | OR | NOT
  TOKEN_COMMA = 1 << 17,     // 262144 : ,
  TOKEN_COLON = 1 << 18,     // 524288 : :
  TOKEN_FULLSTOP = 1 << 19,  // 1048576 : .
  TOKEN_PIPE = 1 << 20,      // 2097152 : |
  TOKEN_EOF = 1 << 21        // 4194304 : ^D
};

typedef struct TOKEN_STRUCT
{
  enum token_type type;
  char *value;
} token_;

const char *token_type_to_string(enum token_type types);
token_ *init_token(enum token_type type, char *value);

#endif
