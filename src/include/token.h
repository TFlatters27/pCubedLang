#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
  TOKEN_ID,
  TOKEN_ASSIGNMENT, // lexer_next : <-
  TOKEN_ARRAY,
  TOKEN_INT,
  TOKEN_REAL,
  TOKEN_NEWLINE,
  TOKEN_LPAREN,   // lexer_next : (
  TOKEN_RPAREN,   // lexer_next : )
  TOKEN_PLUS,     // lexer_next : +
  TOKEN_MINUS,    // lexer_next : -
  TOKEN_MUL,      // lexer_next : *
  TOKEN_DIV,      // lexer_next : /
  TOKEN_LT,       // lexer_next : <
  TOKEN_GT,       // lexer_next : >
  TOKEN_LE,       // lexer_next : <=
  TOKEN_GE,       // lexer_next : >=
  TOKEN_EQ,       // lexer_next : =
  TOKEN_NE,       // lexer_next : !=
  TOKEN_COMMA,    // lexer_next : ,
  TOKEN_COLON,    // lexer_next : :
  TOKEN_FULLSTOP, // lexer_next : .
  TOKEN_LBRACKET, // lexer_next : [
  TOKEN_RBRACKET, // lexer_next : ]
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
