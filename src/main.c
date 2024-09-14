#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/scope.h"
#include "include/io.h"
#include "include/interpreter.h"

#define MAX_LIMIT 20

void print_help()
{
  printf("Usage:\np3 <filename>\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc >= 2)
  {
    for (int i = 1; i < argc; i++)
    {
      int len = strlen(argv[i]);
      char *extension = &argv[i][len - 3]; // For ".p3" extension
      if (strcmp(extension, ".p3") == 0)
      {
        char *file_contents = get_file_contents(argv[i]);

        // Initialize components
        lexer_ *lexer = init_lexer(file_contents);
        scope_ *scope = init_scope();
        parser_ *parser = init_parser(lexer, scope);
        interpreter_ *interpreter = init_interpreter();

        // Parse and interpret
        ast_ *root = parser_parse(parser, scope);
        interpreter_process(interpreter, root);
      }
      else
      {
        print_help();
      }
    }
  }
  else
  {
    char input[MAX_LIMIT];
    printf("Welcome to the P-cubed language v.1.0.0\nCreated by mxcury\nTo exit REPL mode call `>>> EXIT`\n");
    while (1)
    {
      printf(">>> ");
      fgets(input, MAX_LIMIT, stdin);

      // Initialize components for REPL mode
      lexer_ *lexer = init_lexer(input);
      scope_ *scope = init_scope();
      parser_ *parser = init_parser(lexer, scope);
      interpreter_ *interpreter = init_interpreter();

      // Parse and interpret user input
      ast_ *root = parser_parse(parser, scope);
      interpreter_process(interpreter, root);
    }
  }
  return 0;
}
