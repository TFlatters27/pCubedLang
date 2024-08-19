#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/scope.h"
#include "include/io.h"

int main(int argc, char *argv[])
{
  char *file_contents = get_file_contents(argv[1]);

  // Initialize the lexer with the file contents
  lexer_ *lexer = init_lexer(file_contents);
  parser_ *parser = init_parser(lexer);
  scope_ *scope = init_scope();
  ast_ *ast = parser_parse(parser, scope);

  printf("%d\n", ast->type);
  printf("%zu\n", ast->compound_size);

  return 0;
}
