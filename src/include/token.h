#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
  TOKEN_ID,
  TOKEN_ASSIGNMENT, // lexer_next : <-
  TOKEN_ARRAY,
  TOKEN_STRING,
  TOKEN_INT,
  TOKEN_REAL,
  TOKEN_BOOL,
  TOKEN_CHAR,
  TOKEN_NEWLINE,
  TOKEN_LPAREN, // lexer_next : (
  TOKEN_RPAREN, // lexer_next : )
  TOKEN_BIN_OP,
  TOKEN_COMMA,    // lexer_next : ,
  TOKEN_COLON,    // lexer_next : :
  TOKEN_FULLSTOP, // lexer_next : .
  TOKEN_EOF
};

typedef struct TOKEN_STRUCT
{
  enum token_type type;
  char *value;
} token_;

const char *token_type_to_string(enum token_type type);
token_ *init_token(enum token_type type, char *value);

#endif
