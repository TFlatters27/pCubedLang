#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
  TOKEN_COMMENT,
  TOKEN_IDENTIFIER,
  TOKEN_ASSIGNMENT,
  TOKEN_EXPRESSION,
  TOKEN_STRING_LITERAL,
  TOKEN_KEYWORD,
  TOKEN_WHITESPACE,
  TOKEN_INDENT,
  TOKEN_DEDENT,
  TOKEN_ARITHMETIC_OPERATOR,
  TOKEN_RELATIONAL_OPERATOR,
  TOKEN_BOOLEAN_OPERATOR,
  TOKEN_PARENTHESIS,
  TOKEN_PUNCTUATION,
  TOKEN_NUMBER_LITERAL,
  TOKEN_END_OF_LINE,
};

typedef struct TOKEN_STRUCT
{
  enum token_type type;
  char *value;
} token_;

const char *token_type_to_string(enum token_type type);
token_ *init_token(enum token_type type, char *value);

#endif
