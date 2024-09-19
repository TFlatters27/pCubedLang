#include "include/token.h"
#include <stdlib.h>
#include <string.h>

const char *token_type_to_string(enum token_type types)
{
  static char buffer[256];
  buffer[0] = '\0'; // Start with an empty string

  const struct
  {
    enum token_type type;
    const char *name;
  } token_map[] = {
      {TOKEN_ID, "ID"},
      {TOKEN_ASSIGNMENT, "ASSIGNMENT"},
      {TOKEN_STRING, "STRING"},
      {TOKEN_INT, "INTEGER"},
      {TOKEN_REAL, "REAL"},
      {TOKEN_BOOL, "BOOLEAN"},
      {TOKEN_CHAR, "CHARACTER"},
      {TOKEN_NEWLINE, "NEWLINE"},
      {TOKEN_LPAREN, "LEFT PARENTHESIS"},
      {TOKEN_RPAREN, "RIGHT PARENTHESIS"},
      {TOKEN_LBRACKET, "LEFT BRACKET"},
      {TOKEN_RBRACKET, "RIGHT BRACKET"},
      {TOKEN_ARITH_OP, "ARITHMETIC OPERATOR"},
      {TOKEN_REL_OP, "RELATIONAL OPERATOR"},
      {TOKEN_BOOL_OP, "BOOLEAN OPERATOR"},
      {TOKEN_COMMA, "COMMA"},
      {TOKEN_COLON, "COLON"},
      {TOKEN_FULLSTOP, "FULLSTOP"},
      {TOKEN_PIPE, "PIPE"},
      {TOKEN_EOF, "END OF FILE"}};

  int remaining_bits = types;

  for (int i = 0; i < sizeof(token_map) / sizeof(token_map[0]); i++)
  {
    if (types & token_map[i].type)
    {
      strcat(buffer, token_map[i].name);
      strcat(buffer, " ");
      remaining_bits &= ~token_map[i].type; // Clear the recognized bit
    }
  }

  if (remaining_bits)
  {
    strcat(buffer, "UNKNOWN");
  }

  // Remove the trailing space
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == ' ')
  {
    buffer[len - 1] = '\0';
  }

  return buffer;
}

token_ *init_token(enum token_type type, char *value)
{
  token_ *token = calloc(1, sizeof(token_));
  token->type = type;
  token->value = value;

  return token;
}
