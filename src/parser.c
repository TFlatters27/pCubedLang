#include "include/parser.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

parser_ *init_parser(lexer_ *lexer)
{
  parser_ *parser = calloc(1, sizeof(struct PARSER_STRUCT));
  parser->lexer = lexer;
  parser->current_token = lexer_next(lexer);
  parser->prev_token = parser->current_token;

  parser->scope = init_scope();

  return parser;
}

void parser_expect(parser_ *parser, enum token_type type)
{
  printf("expected token %s | actual token %s :: %s\n", token_type_to_string(type), token_type_to_string(parser->current_token->type), parser->current_token->value);
  if (parser->current_token->type == type)
  {
    parser->prev_token = parser->current_token;
    parser->current_token = lexer_next(parser->lexer);
  }
  else
  {
    printf(
        "Unexpected token `%s`, with type %s",
        parser->current_token->value,
        token_type_to_string(parser->current_token->type));
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

  while (parser->current_token->type == TOKEN_NEWLINE)
  {
    parser_expect(parser, TOKEN_NEWLINE);

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
  if (parser->current_token->type == TOKEN_ARRAY)
  {
    return parser_parse_string(parser, scope);
  }
  else if (parser->current_token->type == TOKEN_ID)
  {
    return parser_parse_id(parser, scope);
  }

  return init_ast(AST_NOOP);
}

ast_ *parser_parse_factor(parser_ *parser, scope_ *scope)
{
  return init_ast(AST_NOOP);
}

ast_ *parser_parse_term(parser_ *parser, scope_ *scope)
{
  return init_ast(AST_NOOP);
}

ast_ *parser_parse_subroutine_call(parser_ *parser, scope_ *scope)
{
  ast_ *subroutine_call = init_ast(AST_SUBROUTINE_CALL);

  subroutine_call->subroutine_call_name = parser->prev_token->value;
  parser_expect(parser, TOKEN_LPAREN);

  subroutine_call->subroutine_call_arguments = calloc(1, sizeof(struct AST_STRUCT *));

  ast_ *ast_expr = parser_parse_expr(parser, scope);
  subroutine_call->subroutine_call_arguments[0] = ast_expr;
  subroutine_call->subroutine_call_arguments_size += 1;

  while (parser->current_token->type == TOKEN_COMMA)
  {
    parser_expect(parser, TOKEN_COMMA);

    ast_ *ast_expr = parser_parse_expr(parser, scope);
    subroutine_call->subroutine_call_arguments_size += 1;
    subroutine_call->subroutine_call_arguments = realloc(
        subroutine_call->subroutine_call_arguments,
        subroutine_call->subroutine_call_arguments_size * sizeof(struct AST_STRUCT *));
    subroutine_call->subroutine_call_arguments[subroutine_call->subroutine_call_arguments_size - 1] = ast_expr;
  }
  parser_expect(parser, TOKEN_RPAREN);

  subroutine_call->scope = scope;

  return subroutine_call;
}

ast_ *parser_parse_variable_definition(parser_ *parser, scope_ *scope)
{
  char *variable_definition_variable_name = parser->current_token->value;
  parser_expect(parser, TOKEN_ID);
  ast_ *variable_definition_value = parser_parse_expr(parser, scope);

  ast_ *variable_definition = init_ast(AST_VARIABLE_ASSIGNMENT);
  variable_definition->variable_definition_variable_name = variable_definition_variable_name;
  variable_definition->variable_definition_value = variable_definition_value;

  variable_definition->scope = scope;

  return variable_definition;
}

ast_ *parser_parse_subroutine_definition(parser_ *parser, scope_ *scope)
{

  ast_ *ast = init_ast(AST_SUBROUTINE_DEFINITION);

  parser_expect(parser, TOKEN_ID); // SUBROUTINE keyword

  // Parse the subroutine name
  char *subroutine_name = parser->current_token->value;
  ast->subroutine_definition_name = calloc(strlen(subroutine_name) + 1, sizeof(char));
  strcpy(ast->subroutine_definition_name, subroutine_name);
  parser_expect(parser, TOKEN_ID); // Subroutine name

  parser_expect(parser, TOKEN_LPAREN);

  ast->subroutine_definition_args = calloc(1, sizeof(struct AST_STRUCT *));
  ast->subroutine_definition_args_size = 0;

  // Parse arguments if they exist
  if (parser->current_token->type != TOKEN_RPAREN)
  {
    ast_ *arg = parser_parse_variable(parser, scope);
    ast->subroutine_definition_args_size += 1;
    ast->subroutine_definition_args[ast->subroutine_definition_args_size - 1] = arg;

    while (parser->current_token->type == TOKEN_COMMA)
    {
      parser_expect(parser, TOKEN_COMMA);

      ast->subroutine_definition_args_size += 1;
      ast->subroutine_definition_args = realloc(
          ast->subroutine_definition_args,
          ast->subroutine_definition_args_size * sizeof(struct AST_STRUCT *));

      ast_ *arg = parser_parse_variable(parser, scope);
      ast->subroutine_definition_args[ast->subroutine_definition_args_size - 1] = arg;
    }
  }

  parser_expect(parser, TOKEN_RPAREN);

  // Parse the body of the subroutine
  ast->subroutine_definition_body = parser_parse_statements(parser, scope);

  parser_expect(parser, TOKEN_ID); // Consume ENDSUBROUTINE keyword

  ast->scope = scope;

  return ast;
}

ast_ *parser_parse_variable(parser_ *parser, scope_ *scope)
{
  char *token_value = parser->current_token->value;
  parser_expect(parser, TOKEN_ID);

  if (parser->current_token->type == TOKEN_LPAREN)
  {
    printf("parsing subroutine call\n");
    return parser_parse_subroutine_call(parser, scope);
  }

  printf("parsing variable\n");

  ast_ *ast_variable = init_ast(AST_VARIABLE);
  ast_variable->variable_name = token_value;

  ast_variable->scope = scope;

  return ast_variable;
}

ast_ *parser_parse_string(parser_ *parser, scope_ *scope)
{
  printf("parse string: %s\n", parser->current_token->value);
  ast_ *ast_string = init_ast(AST_ARRAY);
  ast_string->string_value = parser->current_token->value;

  parser_expect(parser, TOKEN_ARRAY);

  ast_string->scope = scope;

  return ast_string;
}

ast_ *parser_parse_id(parser_ *parser, scope_ *scope)
{
  if (strcmp(parser->current_token->value, "CONSTANT") == 0)
  {
  }
  return init_ast(AST_NOOP);
}
