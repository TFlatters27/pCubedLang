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

parser_ *init_parser(lexer_ *lexer);

void parser_validate_token(parser_ *parser, int token_type);

ast_ *parser_parse(parser_ *parser, scope_ *scope);

ast_ *parser_parse_statement(parser_ *parser, scope_ *scope);

ast_ *parser_parse_statements(parser_ *parser, scope_ *scope);

ast_ *parser_parse_expr(parser_ *parser, scope_ *scope);

ast_ *parser_parse_factor(parser_ *parser, scope_ *scope);

ast_ *parser_parse_term(parser_ *parser, scope_ *scope);

ast_ *parser_parse_subroutine_call(parser_ *parser, scope_ *scope);

ast_ *parser_parse_variable_definition(parser_ *parser, scope_ *scope);

ast_ *parser_parse_subroutine_definition(parser_ *parser, scope_ *scope);

ast_ *parser_parse_variable(parser_ *parser, scope_ *scope);

ast_ *parser_parse_string(parser_ *parser, scope_ *scope);

ast_ *parser_parse_id(parser_ *parser, scope_ *scope);
#endif