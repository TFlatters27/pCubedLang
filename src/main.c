#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"

int main()
{
  char *file_contents = "OUTPUT \"Enter a number:\"\nINPUT number\nOUTPUT \"Your number is\" + number";

  // Initialize the lexer with the file contents
  lexer_ *lexer = init_lexer(file_contents);
  token_ *token = (void *)0;

  while ((token = lexer_next(lexer)) != (void *)0)
  {
    printf("TOKEN(%d,%s)\n", token->type, token->value);
  }

  return 0;
}
