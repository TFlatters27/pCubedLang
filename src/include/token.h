#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
  TOKEN_ID,
  TOKEN_COMMENT,
  TOKEN_ASSIGNMENT,
  TOKEN_STRING,
  TOKEN_NEWLINE,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_COMMA,
  TOKEN_INDENT,
  TOKEN_DEDENT,
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
