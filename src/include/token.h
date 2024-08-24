#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
  TOKEN_ID = 1 << 0,         // 1
  TOKEN_ASSIGNMENT = 1 << 1, // 2
  TOKEN_ARRAY = 1 << 2,      // 4
  TOKEN_STRING = 1 << 3,     // 8
  TOKEN_INT = 1 << 4,        // 16
  TOKEN_REAL = 1 << 5,       // 32
  TOKEN_BOOL = 1 << 6,       // 64
  TOKEN_CHAR = 1 << 7,       // 128
  TOKEN_NEWLINE = 1 << 8,    // 256
  TOKEN_LPAREN = 1 << 9,     // 512
  TOKEN_RPAREN = 1 << 10,    // 1024
  TOKEN_BIN_OP = 1 << 11,    // 2048
  TOKEN_COMMA = 1 << 12,     // 4096
  TOKEN_COLON = 1 << 13,     // 8192
  TOKEN_FULLSTOP = 1 << 14,  // 16384
  TOKEN_EOF = 1 << 15        // 32768
};

typedef struct TOKEN_STRUCT
{
  enum token_type type;
  char *value;
} token_;

const char *token_type_to_string(enum token_type type);
token_ *init_token(enum token_type type, char *value);

#endif
