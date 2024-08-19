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
  while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '#')
  {
    // Skip comments starting with '#'
    if (lexer->c == '#')
    {
      // Skip characters until a newline or end of string is found
      while (lexer->c != '\n' && lexer->c != '\0')
      {
        lexer_progress(lexer);
      }
    }

    if (lexer->c == '\n')
    {
      lexer_progress(lexer);
      int indent_level = 0;
      int spaces_count = 0;

      // Calculate the indentation level
      while (lexer->c == ' ' || lexer->c == '\t')
      {
        if (lexer->c == '\t')
        {
          indent_level += TAB_WIDTH;
          spaces_count = 0;
        }
        else
        {
          indent_level++;
          spaces_count++;
          if (spaces_count == TAB_WIDTH)
          {
            // Adjust for mixing spaces and tabs
            indent_level -= (TAB_WIDTH - 1);
            spaces_count = 0;
          }
        }

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

      return init_token(TOKEN_NEWLINE, NULL);
    }

    lexer_progress(lexer);
  }

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

    if (isalnum(lexer->c) || lexer->c == '_')
      return lexer_collect_alphanum(lexer);

    if (lexer->c == '"')
      return lexer_collect_str(lexer);

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
      return init_token(TOKEN_LE, "<=");
    }

    if (lexer->c == '>' && lexer->contents[lexer->index + 1] == '=')
    {
      lexer_progress(lexer); // progress past '>'
      lexer_progress(lexer); // progress past '='
      return init_token(TOKEN_GE, ">=");
    }

    if (lexer->c == '!' && lexer->contents[lexer->index + 1] == '=')
    {
      lexer_progress(lexer); // progress past '!'
      lexer_progress(lexer); // progress past '='
      return init_token(TOKEN_NE, "!=");
    }

    switch (lexer->c)
    {
    case '+':
      return lexer_collect_token(lexer, init_token(TOKEN_PLUS, lexer_get_char_as_string(lexer)));
    case '-':
      return lexer_collect_token(lexer, init_token(TOKEN_MINUS, lexer_get_char_as_string(lexer)));
    case '*':
      return lexer_collect_token(lexer, init_token(TOKEN_MUL, lexer_get_char_as_string(lexer)));
    case '/':
      return lexer_collect_token(lexer, init_token(TOKEN_DIV, lexer_get_char_as_string(lexer)));
    case '<':
      return lexer_collect_token(lexer, init_token(TOKEN_LT, lexer_get_char_as_string(lexer)));
    case '>':
      return lexer_collect_token(lexer, init_token(TOKEN_GT, lexer_get_char_as_string(lexer)));
    case '=':
      return lexer_collect_token(lexer, init_token(TOKEN_EQ, lexer_get_char_as_string(lexer)));
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
    }
  }

  return init_token(TOKEN_EOF, "\0");
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

  return init_token(TOKEN_STRING, value);
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

  return init_token(TOKEN_ID, value);
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
