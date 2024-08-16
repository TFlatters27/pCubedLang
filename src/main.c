#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"

int main()
{
  char *file_contents = "OUTPUT \"Enter a number:\"\nINPUT number\nOUTPUT \"Your number is\"";

  // Initialize the lexer with the file contents
  lexer_ *lexer = init_lexer(file_contents);
  token_ *token = NULL;

  while ((token = lexer_next(lexer)) != NULL)
  {
    printf("TOKEN(%s,%s)\n", token_type_to_string(token->type), token->value);
  }

  return 0;
}
