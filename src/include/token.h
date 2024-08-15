#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
  enum token_type
  {
    COMMENT,
    IDENTIFIER,
    ASSIGNMENT,
    EXPRESSION,
    STRING_LITERAL,
    KEYWORD,
    WHITESPACE,
    INDENT,
    DEDENT,
    OPERATOR,
    PARENTHESIS,
    NUMBER_LITERAL,
    END_OF_LINE,
  } type;

  char *value;
} token_;

token_ *init_token(enum token_type type, char *value);

#endif
