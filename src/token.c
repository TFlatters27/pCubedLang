#include "include/token.h"
#include <stdlib.h>

const char *token_type_to_string(enum token_type type)
{
  switch (type)
  {
  case TOKEN_ID:
    return "ID";
  case TOKEN_COMMENT:
    return "COMMENT";
  case TOKEN_ASSIGNMENT:
    return "ASSIGNMENT";
  case TOKEN_STRING:
    return "STRING";
  case TOKEN_NEWLINE:
    return "NEWLINE";
  case TOKEN_LPAREN:
    return "LPAREN";
  case TOKEN_RPAREN:
    return "RPAREN";
  case TOKEN_COMMA:
    return "COMMA";
  case TOKEN_EOF:
    return "EOF";
  default:
    return "UNKNOWN";
  }
}

token_ *init_token(enum token_type type, char *value)
{
  token_ *token = calloc(1, sizeof(token_));
  token->type = type;
  token->value = value;

  return token;
}
