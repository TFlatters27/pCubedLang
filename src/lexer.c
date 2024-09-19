#include "include/lexer.h"
#include "include/token.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/**
 * @brief Initialises the lexer with the contents of the program provided.
 * The lexer is assigned the amount of memory needed for the lexer structure.
 * The lexer is also initialized with index of the first character in the program.
 * The character that is initially assigned is the index of the first character in the program.
 *
 * @param contents
 * @return lexer_*
 */
lexer_ *init_lexer(char *contents)
{
  lexer_ *lexer = calloc(1, sizeof(lexer_));
  lexer->contents = contents;
  lexer->index = 0;
  lexer->c = contents[lexer->index];
  lexer->line = 1;   // Start at line 1
  lexer->column = 1; // Start at column 1

  return lexer;
}

/**
 * @brief Progresses the lexer to the next character in the program
 *
 * @param lexer
 */
void lexer_progress(lexer_ *lexer)
{
  if (lexer->c != '\0' && lexer->index < strlen(lexer->contents))
  {
    lexer->index++;
    lexer->c = lexer->contents[lexer->index];

    if (lexer->c == '\n')
    {
      lexer->line++;
      lexer->column = 1; // Reset column to 1 on a new line
    }
    else
    {
      lexer->column++; // Increment column for every character
    }
  }
}

token_ *lexer_peek(lexer_ *lexer)
{
  // Save the current state of the lexer
  int saved_index = lexer->index;
  char saved_c = lexer->c;

  // Fetch the next token using the existing lexer_next logic
  token_ *next_token = lexer_next(lexer);

  // Restore the lexer's state to what it was before peeking
  lexer->index = saved_index;
  lexer->c = saved_c;

  return next_token;
}

/**
 * @brief This skips the lexer over whitespace characters and calls the progress function as if the lexer has parsed the current character.
 *
 * @param lexer
 */
token_ *lexer_skip(lexer_ *lexer)
{
  while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '#')
  {
    // Skip whitespace
    while (lexer->c == ' ' || lexer->c == '\t')
    {
      lexer_progress(lexer);
    }

    // Handle newlines by returning a TOKEN_NEWLINE
    if (lexer->c == '\n')
    {
      lexer_progress(lexer);
      return init_token(TOKEN_NEWLINE, "\\n");
    }

    // Skip comments starting with '#'
    if (lexer->c == '#')
    {
      // Skip characters until a newline or end of string is found
      while (lexer->c != '\n' && lexer->c != '\0')
      {
        lexer_progress(lexer);
      }
    }
  }

  // If nothing to skip, return NULL (indicating no more tokens to skip)
  return NULL;
}

token_ *lexer_next(lexer_ *lexer)
{
  while (lexer->c != '\0' && lexer->index < strlen(lexer->contents))
  {
    // Skip spaces, tabs, newlines, and comments
    if (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '#')
    {
      token_ *skipped = lexer_skip(lexer);
      if (skipped)
        return skipped;
    }

    if (isdigit(lexer->c) || (lexer->c == '.' && isdigit(lexer->contents[lexer->index + 1])))
      return lexer_collect_number(lexer);

    if (isalnum(lexer->c) || lexer->c == '_')
      return lexer_collect_alphanum(lexer);

    if (lexer->c == '"' || lexer->c == '\'')
      return lexer_collect_string(lexer);

    // Handle multi-character operators and special cases
    if (lexer->c == '<' && lexer->contents[lexer->index + 1] == '-')
    {
      lexer_progress(lexer); // progress past '<'
      lexer_progress(lexer); // progress past '-'
      return init_token(TOKEN_ASSIGNMENT, "<-");
    }

    if (lexer->c == '<' && lexer->contents[lexer->index + 1] == '=')
    {
      lexer_progress(lexer); // progress past '<'
      lexer_progress(lexer); // progress past '='
      return init_token(TOKEN_REL_OP, "<=");
    }

    if (lexer->c == '>' && lexer->contents[lexer->index + 1] == '=')
    {
      lexer_progress(lexer); // progress past '>'
      lexer_progress(lexer); // progress past '='
      return init_token(TOKEN_REL_OP, ">=");
    }

    if (lexer->c == '!' && lexer->contents[lexer->index + 1] == '=')
    {
      lexer_progress(lexer); // progress past '!'
      lexer_progress(lexer); // progress past '='
      return init_token(TOKEN_REL_OP, "!=");
    }

    switch (lexer->c)
    {
    case '+':
      return lexer_collect_token(lexer, init_token(TOKEN_ARITH_OP, lexer_get_char_as_string(lexer)));
    case '-':
      return lexer_collect_token(lexer, init_token(TOKEN_ARITH_OP, lexer_get_char_as_string(lexer)));
    case '*':
      return lexer_collect_token(lexer, init_token(TOKEN_ARITH_OP, lexer_get_char_as_string(lexer)));
    case '/':
      return lexer_collect_token(lexer, init_token(TOKEN_ARITH_OP, lexer_get_char_as_string(lexer)));
    case '^':
      return lexer_collect_token(lexer, init_token(TOKEN_ARITH_OP, lexer_get_char_as_string(lexer)));
    case '<':
      return lexer_collect_token(lexer, init_token(TOKEN_REL_OP, lexer_get_char_as_string(lexer)));
    case '>':
      return lexer_collect_token(lexer, init_token(TOKEN_REL_OP, lexer_get_char_as_string(lexer)));
    case '=':
      return lexer_collect_token(lexer, init_token(TOKEN_REL_OP, lexer_get_char_as_string(lexer)));
    case '(':
      return lexer_collect_token(lexer, init_token(TOKEN_LPAREN, lexer_get_char_as_string(lexer)));
    case ')':
      return lexer_collect_token(lexer, init_token(TOKEN_RPAREN, lexer_get_char_as_string(lexer)));
    case '[':
      return lexer_collect_token(lexer, init_token(TOKEN_LBRACKET, lexer_get_char_as_string(lexer)));
    case ']':
      return lexer_collect_token(lexer, init_token(TOKEN_RBRACKET, lexer_get_char_as_string(lexer)));
    case ',':
      return lexer_collect_token(lexer, init_token(TOKEN_COMMA, lexer_get_char_as_string(lexer)));
    case ':':
      return lexer_collect_token(lexer, init_token(TOKEN_COLON, lexer_get_char_as_string(lexer)));
    case '.':
      return lexer_collect_token(lexer, init_token(TOKEN_FULLSTOP, lexer_get_char_as_string(lexer)));
    case '|':
      return lexer_collect_token(lexer, init_token(TOKEN_PIPE, lexer_get_char_as_string(lexer)));
    }
  }

  return init_token(TOKEN_EOF, "\0");
}

token_ *lexer_collect_string(lexer_ *lexer)
{
  if (lexer->c == '"' || lexer->c == '\'')
  {
    char quote_char = lexer->c;
    lexer_progress(lexer); // Move past the initial quote character

    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lexer->c != quote_char && lexer->c != '\0')
    {
      char *s = lexer_get_char_as_string(lexer);
      value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
      strcat(value, s);
      lexer_progress(lexer);
    }

    lexer_progress(lexer); // Move past the closing quote character

    if (quote_char == '\'' && strlen(value) == 1)
    {
      return init_token(TOKEN_CHAR, value);
    }
    else
    {
      return init_token(TOKEN_STRING, value);
    }
  }
  return NULL; // Return NULL if no string/char is detected
}

token_ *lexer_collect_alphanum(lexer_ *lexer)
{
  char *value = calloc(1, sizeof(char));
  value[0] = '\0';

  // Loop while the character is alphanumeric or an underscore
  while (isalnum(lexer->c) || lexer->c == '_')
  {
    char *s = lexer_get_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);

    lexer_progress(lexer);
  }

  // Check if the collected value is "True" or "False"
  if (strcmp(value, "True") == 0 || strcmp(value, "False") == 0)
  {
    return init_token(TOKEN_BOOL, value);
  }

  if (strcmp(value, "DIV") == 0 || strcmp(value, "MOD") == 0)
  {
    return init_token(TOKEN_ARITH_OP, value);
  }

  if (strcmp(value, "AND") == 0 || strcmp(value, "OR") == 0 || strcmp(value, "NOT") == 0)
  {
    return init_token(TOKEN_BOOL_OP, value);
  }

  return init_token(TOKEN_ID, value);
}

token_ *lexer_collect_number(lexer_ *lexer)
{
  char *value = calloc(1, sizeof(char));
  value[0] = '\0';

  int has_decimal_point = 0;

  while (isdigit(lexer->c) || lexer->c == '.')
  {
    if (lexer->c == '.')
    {
      if (has_decimal_point)
        break;
      has_decimal_point = 1;
    }

    char *s = lexer_get_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);

    lexer_progress(lexer);
  }

  // Check for the exponent part (e.g., "e+2" or "E-3")
  if (lexer->c == 'e' || lexer->c == 'E')
  {
    char *s = lexer_get_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);
    lexer_progress(lexer);

    if (lexer->c == '+' || lexer->c == '-')
    {
      s = lexer_get_char_as_string(lexer);
      value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
      strcat(value, s);
      lexer_progress(lexer);
    }

    while (isdigit(lexer->c))
    {
      s = lexer_get_char_as_string(lexer);
      value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
      strcat(value, s);
      lexer_progress(lexer);
    }
  }

  if (has_decimal_point)
  {
    return init_token(TOKEN_REAL, value);
  }
  else
  {
    return init_token(TOKEN_INT, value);
  }
}

token_ *lexer_collect_token(lexer_ *lexer, token_ *token)
{
  lexer_progress(lexer);
  return token;
}

char *lexer_get_char_as_string(lexer_ *lexer)
{
  char *str = calloc(2, sizeof(char));
  str[0] = lexer->c;
  str[1] = '\0';

  return str;
}
