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
  case TOKEN_STRING:
    return "STRING";
  case TOKEN_NEWLINE:
    return "NEWLINE";
  case TOKEN_LPAREN:
    return "LPAREN";
  case TOKEN_RPAREN:
    return "RPAREN";
  case TOKEN_PLUS:
    return "PLUS";
  case TOKEN_MINUS:
    return "MINUS";
  case TOKEN_MUL:
    return "MUL";
  case TOKEN_DIV:
    return "DIV";
  case TOKEN_LT:
    return "LT"; // Less than
  case TOKEN_GT:
    return "GT"; // Greater than
  case TOKEN_LE:
    return "LE"; // Less than or equal to
  case TOKEN_GE:
    return "GE"; // Greater than or equal to
  case TOKEN_EQ:
    return "EQ"; // Equal to
  case TOKEN_NE:
    return "NE"; // Not equal to
  case TOKEN_COMMA:
    return "COMMA";
  case TOKEN_COLON:
    return "COLON";
  case TOKEN_LBRACKET:
    return "LBRACKET"; // Left bracket
  case TOKEN_RBRACKET:
    return "RBRACKET"; // Right bracket
  case TOKEN_INDENT:
    return "INDENT";
  case TOKEN_DEDENT:
    return "DEDENT";
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
