#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include "scope.h"

typedef struct PARSER_STRUCT
{
  lexer_ *lexer;
  token_ *current_token;
  token_ *prev_token;
  scope_ *scope;
} parser_;

typedef ast_ *(*handler_function)(parser_ *parser, scope_ *scope);

typedef struct
{
  const char *keyword;
  handler_function handler;
} keyword_map_entry;

// Function prototypes
parser_ *init_parser(lexer_ *lexer);

void parser_expect(parser_ *parser, enum token_type expected_type);

ast_ *parser_parse(parser_ *parser, scope_ *scope);

ast_ *parser_parse_statement(parser_ *parser, scope_ *scope);

ast_ *parser_parse_statements(parser_ *parser, scope_ *scope);

ast_ *parser_parse_id(parser_ *parser, scope_ *scope);

// Additional function prototypes
ast_ **init_ast_list(void);
void add_ast_to_list(ast_ ***list, ast_ *new_ast);

ast_ *parse_expression(parser_ *parser, scope_ *scope);

ast_ *handle_id(parser_ *parser, scope_ *scope);
ast_ *handle_undefined_loop(parser_ *parser, scope_ *scope);
ast_ *handle_defined_loop(parser_ *parser, scope_ *scope);
ast_ *handle_selection(parser_ *parser, scope_ *scope);
ast_ *handle_record(parser_ *parser, scope_ *scope);
ast_ *handle_subroutine(parser_ *parser, scope_ *scope);
ast_ *handle_return(parser_ *parser, scope_ *scope);
ast_ *handle_output(parser_ *parser, scope_ *scope);
ast_ *handle_userinput(parser_ *parser, scope_ *scope);

#endif
