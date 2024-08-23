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
  case TOKEN_INT:
    return "INTEGER";
  case TOKEN_REAL:
    return "REAL";
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
    return "MULTIPLY";
  case TOKEN_DIV:
    return "DIVIDE";
  case TOKEN_LT:
    return "LESS THAN"; // Less than
  case TOKEN_GT:
    return "GREATER THAN"; // Greater than
  case TOKEN_LE:
    return "LESS THAN OR EQUAL"; // Less than or equal to
  case TOKEN_GE:
    return "GREATER THAN OR EQUAL"; // Greater than or equal to
  case TOKEN_EQ:
    return "EQUAL"; // Equal to
  case TOKEN_NE:
    return "NOT EQUAL"; // Not equal to
  case TOKEN_COMMA:
    return "COMMA";
  case TOKEN_COLON:
    return "COLON";
  case TOKEN_FULLSTOP:
    return "FULLSTOP";
  case TOKEN_LBRACKET:
    return "LBRACKET"; // Left bracket
  case TOKEN_RBRACKET:
    return "RBRACKET"; // Right bracket
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
