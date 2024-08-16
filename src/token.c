#include "include/token.h"
#include <stdlib.h>

const char *token_type_to_string(enum token_type type)
{
  switch (type)
  {
  case TOKEN_COMMENT:
    return "COMMENT";
  case TOKEN_IDENTIFIER:
    return "IDENTIFIER";
  case TOKEN_ASSIGNMENT:
    return "ASSIGNMENT";
  case TOKEN_EXPRESSION:
    return "EXPRESSION";
  case TOKEN_STRING_LITERAL:
    return "STRING_LITERAL";
  case TOKEN_KEYWORD:
    return "KEYWORD";
  case TOKEN_WHITESPACE:
    return "WHITESPACE";
  case TOKEN_INDENT:
    return "INDENT";
  case TOKEN_DEDENT:
    return "DEDENT";
  case TOKEN_ARITHMETIC_OPERATOR:
    return "ARITHMETIC_OPERATOR";
  case TOKEN_RELATIONAL_OPERATOR:
    return "RELATIONAL_OPERATOR";
  case TOKEN_BOOLEAN_OPERATOR:
    return "BOOLEAN_OPERATOR";
  case TOKEN_PARENTHESIS:
    return "PARENTHESIS";
  case TOKEN_PUNCTUATION:
    return "PUNCTUATION";
  case TOKEN_NUMBER_LITERAL:
    return "NUMBER_LITERAL";
  case TOKEN_END_OF_LINE:
    return "END_OF_LINE";
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
