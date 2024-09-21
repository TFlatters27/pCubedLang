#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/scope.h"
#include "include/io.h"
#include "include/interpreter.h"

#define MAX_LIMIT 128

void print_help()
{
  printf("Usage:\np3 <filename> [--debug]\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  clock_t start_time = clock(); // Start the clock
  srand(time(NULL));
  int debug = 0;

  // Check if --debug is present
  if (argc >= 2)
  {
    for (int i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "--debug") == 0)
      {
        debug = 1; // Set debug flag if --debug is specified
        break;
      }
    }

    for (int i = 1; i < argc; i++)
    {
      int len = strlen(argv[i]);
      char *extension = &argv[i][len - 3]; // For ".p3" extension
      if (strcmp(extension, ".p3") == 0)
      {
        char *file_contents = get_file_contents(argv[i]);

        // Initialize components
        lexer_ *lexer = init_lexer(file_contents);
        scope_ *scope = init_scope(NULL, "global_scope");
        token_ *token = NULL;
        parser_ *parser = init_parser(lexer, scope);
        interpreter_ *interpreter = init_interpreter();

        // Parse and interpret
        ast_ *root = parser_parse(parser, scope);

        // If debug flag is set, print the root AST
        if (debug)
        {
          printf("**************************ROOT AST*************************\n");
          print_ast(root, 0);
          printf("***********************************************************\n");
        }

        interpreter_process(interpreter, root);
      }
      else if (strcmp(argv[i], "--debug") != 0) // Ignore the --debug flag during extension check
      {
        print_help();
      }
    }
  }
  else
  {
    char input[MAX_LIMIT];
    printf("Welcome to the P-cubed language v.1.0.0\nCreated by mxcury\nTo exit REPL mode call `>>> EXIT`\n");
    scope_ *scope = init_scope(NULL, "global_scope");
    while (1)
    {
      printf(">>> ");
      fgets(input, MAX_LIMIT, stdin);

      // Initialize components for REPL mode
      lexer_ *lexer = init_lexer(input);
      parser_ *parser = init_parser(lexer, scope);
      interpreter_ *interpreter = init_interpreter();

      // Parse and interpret user input
      ast_ *root = parser_parse(parser, scope);
      interpreter_process(interpreter, root);
    }
  }

  // End the clock and calculate the elapsed time
  clock_t end_time = clock();
  double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  // If debug flag is set, print the execution time
  if (debug)
  {
    printf("*************************RUNTIME***************************\n");
    printf("Execution time: %.6f seconds\n", time_spent);
    printf("***********************************************************\n");
  }

  return 0;
}
