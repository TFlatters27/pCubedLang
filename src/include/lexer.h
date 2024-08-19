#ifndef LEXER_H
#define LEXER_H
#include "token.h"

typedef struct LEXER_STRUCT
{
  char *contents;
  unsigned int index;
  char c;
  int current_indent_level;
  int previous_indent_level;
} lexer_;

lexer_ *init_lexer(char *contents);

void lexer_progress(lexer_ *lexer);

token_ *lexer_skip(lexer_ *lexer);

token_ *lexer_next(lexer_ *lexer);

token_ *lexer_collect_str(lexer_ *lexer);

token_ *lexer_collect_alphanum(lexer_ *lexer);

token_ *lexer_collect_token(lexer_ *lexer, token_ *token);

char *lexer_get_char_as_string(lexer_ *lexer);
#endif
