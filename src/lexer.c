#include "include/lexer.h"
#include "include/token.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define TAB_WIDTH 2

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
  lexer->current_indent_level = 0;
  lexer->previous_indent_level = 0;

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
  }
}

/**
 * @brief This skips the lexer over whitespace characters and calls the progress function as if the lexer has parsed the current character.
 * It also handles indentation, dedentation, and newlines, emitting the corresponding tokens.
 *
 * @param lexer
 */
token_ *lexer_skip(lexer_ *lexer)
{
  while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n')
  {
    if (lexer->c == '\n')
    {
      lexer_progress(lexer);
      int indent_level = 0;
      while (lexer->c == ' ' || lexer->c == '\t')
      {
        indent_level += (lexer->c == '\t') ? TAB_WIDTH : 1;
        lexer_progress(lexer);
      }
      lexer->previous_indent_level = lexer->current_indent_level;
      lexer->current_indent_level = indent_level;

      if (lexer->current_indent_level > lexer->previous_indent_level)
      {
        return init_token(TOKEN_INDENT, NULL);
      }
      else if (lexer->current_indent_level < lexer->previous_indent_level)
      {
        return init_token(TOKEN_DEDENT, NULL);
      }

      return init_token(TOKEN_END_OF_LINE, NULL);
    }

    lexer_progress(lexer);
  }

  return NULL;
}

token_ *lexer_next(lexer_ *lexer)
{
  while (lexer->c != '\0' && lexer->index < strlen(lexer->contents))
  {
    if (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n')
    {
      token_ *skipped = lexer_skip(lexer);
      if (skipped)
        return skipped;
    }

    if (isalnum(lexer->c))
      return lexer_collect_alphanum(lexer);

    if (lexer->c == '"')
      return lexer_collect_str(lexer);

    // Handle the assignment operator '<-'
    if (lexer->c == '<' && lexer->contents[lexer->index + 1] == '-')
    {
      lexer_progress(lexer); // progress past '<'
      lexer_progress(lexer); // progress past '-'
      return init_token(TOKEN_ASSIGNMENT, "<-");
    }

    switch (lexer->c)
    {
    case '=':
      return lexer_collect_token(lexer, init_token(TOKEN_RELATIONAL_OPERATOR, lexer_get_char_as_string(lexer)));
      break;
    case '(':
      return lexer_collect_token(lexer, init_token(TOKEN_PARENTHESIS, lexer_get_char_as_string(lexer)));
      break;
    case ')':
      return lexer_collect_token(lexer, init_token(TOKEN_PARENTHESIS, lexer_get_char_as_string(lexer)));
      break;
    case ',':
      return lexer_collect_token(lexer, init_token(TOKEN_RELATIONAL_OPERATOR, lexer_get_char_as_string(lexer)));
      break;
    }
  }

  return NULL;
}

token_ *lexer_collect_str(lexer_ *lexer)
{
  lexer_progress(lexer);

  char *value = calloc(1, sizeof(char));
  value[0] = '\0';

  while (lexer->c != '"' && lexer->c != '\0')
  {
    char *s = lexer_get_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);

    lexer_progress(lexer);
  }

  lexer_progress(lexer);

  return init_token(TOKEN_STRING_LITERAL, value);
}

token_ *lexer_collect_alphanum(lexer_ *lexer)
{
  char *value = calloc(1, sizeof(char));
  value[0] = '\0';

  while (isalnum(lexer->c))
  {
    char *s = lexer_get_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);

    lexer_progress(lexer);
  }

  return init_token(TOKEN_IDENTIFIER, value);
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
