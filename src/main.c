#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/scope.h"
#include "include/io.h"
#include "include/interpreter.h"

int main(int argc, char *argv[])
{
  char *file_contents = get_file_contents(argv[1]);

  // Initialize the lexer with the file contents
  lexer_ *lexer = init_lexer(file_contents);

  // token_ *token = NULL;
  // while ((token = lexer_next(lexer))->type != TOKEN_EOF)
  // {
  //   printf("{%s::%s}\n", token_type_to_string(token->type), token->value);
  // }

  parser_ *parser = init_parser(lexer);
  scope_ *scope = init_scope();
  ast_ *ast = parser_parse(parser, scope);

  printf("AST Type: %s\n", ast_type_to_string(ast->type));

  return 0;
}
