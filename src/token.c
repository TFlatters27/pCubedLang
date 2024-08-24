#include "include/token.h"
#include <stdlib.h>

const char *token_type_to_string(enum token_type type)
{
  switch (type)
  {
  case TOKEN_ID:
    return "ID";
  case TOKEN_ASSIGNMENT:
    return "ASSIGNMENT";
  case TOKEN_ARRAY:
    return "ARRAY";
  case TOKEN_STRING:
    return "STRING";
  case TOKEN_INT:
    return "INTEGER";
  case TOKEN_REAL:
    return "REAL";
  case TOKEN_BOOL:
    return "BOOLEAN";
  case TOKEN_CHAR:
    return "CHARACTER";
  case TOKEN_NEWLINE:
    return "NEWLINE";
  case TOKEN_LPAREN:
    return "LPAREN";
  case TOKEN_RPAREN:
    return "RPAREN";
  case TOKEN_BIN_OP:
    return "BIN_OP";
  case TOKEN_COMMA:
    return "COMMA";
  case TOKEN_COLON:
    return "COLON";
  case TOKEN_FULLSTOP:
    return "FULLSTOP";
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
