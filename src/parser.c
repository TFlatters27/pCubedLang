#include "include/parser.h"
// #include "include/scope.h"
#include <stdio.h>
#include <string.h>

parser_ *init_parser(lexer_ *lexer)
{
  parser_ *parser = calloc(1, sizeof(struct PARSER_STRUCT));
  parser->lexer = lexer;
  parser->current_token = lexer_get_next_token(lexer);
  parser->prev_token = parser->current_token;

  // parser->scope = init_scope();

  return parser;
}

void parser_validate_token(parser_ *parser, int token_type)
{
  if (parser->current_token->type == token_type)
  {
    parser->prev_token = parser->current_token;
    parser->current_token = lexer_get_next_token(parser->lexer);
  }
  else
  {
    printf(
        "Unexpected token `%s`, with type %d",
        parser->current_token->value,
        parser->current_token->type);
    exit(1);
  }
}

ast_ *parser_parse(parser_ *parser, scope_ *scope)
{
  return parser_parse_statements(parser, scope);
}

ast_ *parser_parse_statement(parser_ *parser, scope_ *scope)
{
  switch (parser->current_token->type)
  {
  case TOKEN_ID:
    return parser_parse_id(parser, scope);
  }

  return init_ast(AST_NOOP);
}

ast_ *parser_parse_statements(parser_ *parser, scope_ *scope)
{
  ast_ *compound = init_ast(AST_COMPOUND);
  compound->scope = scope;
  compound->compound_value = calloc(1, sizeof(struct AST_STRUCT *));

  ast_ *ast_statement = parser_parse_statement(parser, scope);
  ast_statement->scope = scope;
  compound->compound_value[0] = ast_statement;
  compound->compound_size += 1;

  while (parser->current_token->type == TOKEN_SEMI)
  {
    parser_eat(parser, TOKEN_SEMI);

    ast_ *ast_statement = parser_parse_statement(parser, scope);

    if (ast_statement)
    {
      compound->compound_size += 1;
      compound->compound_value = realloc(
          compound->compound_value,
          compound->compound_size * sizeof(struct AST_STRUCT *));
      compound->compound_value[compound->compound_size - 1] = ast_statement;
    }
  }

  return compound;
}

ast_ *parser_parse_expr(parser_ *parser, scope_ *scope)
{
  switch (parser->current_token->type)
  {
  case TOKEN_STRING:
    return parser_parse_string(parser, scope);
  case TOKEN_ID:
    return parser_parse_id(parser, scope);
  }

  return init_ast(AST_NOOP);
}

ast_ *parser_parse_factor(parser_ *parser, scope_ *scope)
{
}

ast_ *parser_parse_term(parser_ *parser, scope_ *scope)
{
}

ast_ *parser_parse_function_call(parser_ *parser, scope_ *scope)
{
  ast_ *function_call = init_ast(AST_FUNCTION_CALL);

  function_call->function_call_name = parser->prev_token->value;
  parser_eat(parser, TOKEN_LPAREN);

  function_call->function_call_arguments = calloc(1, sizeof(struct AST_STRUCT *));

  ast_ *ast_expr = parser_parse_expr(parser, scope);
  function_call->function_call_arguments[0] = ast_expr;
  function_call->function_call_arguments_size += 1;

  while (parser->current_token->type == TOKEN_COMMA)
  {
    parser_eat(parser, TOKEN_COMMA);

    ast_ *ast_expr = parser_parse_expr(parser, scope);
    function_call->function_call_arguments_size += 1;
    function_call->function_call_arguments = realloc(
        function_call->function_call_arguments,
        function_call->function_call_arguments_size * sizeof(struct AST_STRUCT *));
    function_call->function_call_arguments[function_call->function_call_arguments_size - 1] = ast_expr;
  }
  parser_eat(parser, TOKEN_RPAREN);

  function_call->scope = scope;

  return function_call;
}

ast_ *parser_parse_variable_definition(parser_ *parser, scope_ *scope)
{
  parser_eat(parser, TOKEN_ID); // var
  char *variable_definition_variable_name = parser->current_token->value;
  parser_eat(parser, TOKEN_ID); // var name
  parser_eat(parser, TOKEN_EQUALS);
  ast_ *variable_definition_value = parser_parse_expr(parser, scope);

  ast_ *variable_definition = init_ast(AST_VARIABLE_DEFINITION);
  variable_definition->variable_definition_variable_name = variable_definition_variable_name;
  variable_definition->variable_definition_value = variable_definition_value;

  variable_definition->scope = scope;

  return variable_definition;
}

ast_ *parser_parse_function_definition(parser_ *parser, scope_ *scope)
{
  ast_ *ast = init_ast(AST_FUNCTION_DEFINITION);
  parser_eat(parser, TOKEN_ID); // function

  char *function_name = parser->current_token->value;
  ast->function_definition_name = calloc(
      strlen(function_name) + 1, sizeof(char));
  strcpy(ast->function_definition_name, function_name);

  parser_eat(parser, TOKEN_ID); // function name

  parser_eat(parser, TOKEN_LPAREN);

  ast->function_definition_args =
      calloc(1, sizeof(struct AST_STRUCT *));

  ast_ *arg = parser_parse_variable(parser, scope);
  ast->function_definition_args_size += 1;
  ast->function_definition_args[ast->function_definition_args_size - 1] = arg;

  while (parser->current_token->type == TOKEN_COMMA)
  {
    parser_eat(parser, TOKEN_COMMA);

    ast->function_definition_args_size += 1;

    ast->function_definition_args =
        realloc(
            ast->function_definition_args,
            ast->function_definition_args_size * sizeof(struct AST_STRUCT *));

    ast_ *arg = parser_parse_variable(parser, scope);
    ast->function_definition_args[ast->function_definition_args_size - 1] = arg;
  }

  parser_eat(parser, TOKEN_RPAREN);

  parser_eat(parser, TOKEN_LBRACE);

  ast->function_definition_body = parser_parse_statements(parser, scope);

  parser_eat(parser, TOKEN_RBRACE);

  ast->scope = scope;

  return ast;
}

ast_ *parser_parse_variable(parser_ *parser, scope_ *scope)
{
  char *token_value = parser->current_token->value;
  parser_eat(parser, TOKEN_ID); // var name or function call name

  if (parser->current_token->type == TOKEN_LPAREN)
    return parser_parse_function_call(parser, scope);

  ast_ *ast_variable = init_ast(AST_VARIABLE);
  ast_variable->variable_name = token_value;

  ast_variable->scope = scope;

  return ast_variable;
}

ast_ *parser_parse_string(parser_ *parser, scope_ *scope)
{
  ast_ *ast_string = init_ast(AST_STRING);
  ast_string->string_value = parser->current_token->value;

  parser_eat(parser, TOKEN_STRING);

  ast_string->scope = scope;

  return ast_string;
}

ast_ *parser_parse_id(parser_ *parser, scope_ *scope)
{
  if (strcmp(parser->current_token->value, "var") == 0)
  {
    return parser_parse_variable_definition(parser, scope);
  }
  else if (strcmp(parser->current_token->value, "function") == 0)
  {
    return parser_parse_function_definition(parser, scope);
  }
  else
  {
    return parser_parse_variable(parser, scope);
  }
}