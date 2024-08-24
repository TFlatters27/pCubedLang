#include "include/parser.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

ast_ *handle_constant(parser_ *parser, scope_ *scope)
{
  printf("Processing a CONSTANT block\n");

  parser_expect(parser, TOKEN_ID); // {ID: CONSTANT}

  ast_ *ast_variable = init_ast(AST_VARIABLE);
  ast_variable->variable_assignment_variable_name = parser->current_token->value;

  parser_expect(parser, TOKEN_ID);         // {ID: variable_name}
  parser_expect(parser, TOKEN_ASSIGNMENT); // {ASSIGNMENT: <-}

  ast_variable->variable_assignment_value = parser->current_token->value;
  parser_expect(parser, TOKEN_STRING | TOKEN_CHAR | TOKEN_BOOL | TOKEN_INT | TOKEN_REAL | TOKEN_ARRAY);

  ast_variable->scope = scope;

  parser_expect(parser, TOKEN_NEWLINE);

  printf("CONSTANT %s <- %s\n", ast_variable->variable_assignment_variable_name, ast_variable->variable_assignment_value);

  return ast_variable;
}

ast_ *handle_repeat(parser_ *parser, scope_ *scope)
{
  printf("Processing a REPEAT block\n");
  return init_ast(AST_NOOP);
}

ast_ *handle_while(parser_ *parser, scope_ *scope)
{
  printf("Processing a WHILE block\n");
  return init_ast(AST_NOOP);
}

ast_ *handle_for(parser_ *parser, scope_ *scope)
{
  printf("Processing a FOR block\n");
  return init_ast(AST_NOOP);
}

ast_ *handle_if(parser_ *parser, scope_ *scope)
{
  printf("Processing an IF block\n");
  return init_ast(AST_NOOP);
}

ast_ *handle_record(parser_ *parser, scope_ *scope)
{
  printf("Processing a RECORD block\n");
  return init_ast(AST_NOOP);
}

ast_ *handle_subroutine(parser_ *parser, scope_ *scope)
{
  printf("Processing a SUBROUTINE block\n");
  return init_ast(AST_NOOP);
}

ast_ *handle_return(parser_ *parser, scope_ *scope)
{
  printf("Processing a RETURN block\n");
  return init_ast(AST_NOOP);
}

ast_ *handle_output(parser_ *parser, scope_ *scope)
{
  printf("Processing an OUTPUT block\n");
  return init_ast(AST_NOOP);
}

ast_ *handle_undefined(parser_ *parser, scope_ *scope)
{
  printf("Processing an undefined block\n");
  return init_ast(AST_NOOP);
}

parser_ *init_parser(lexer_ *lexer)
{
  parser_ *parser = calloc(1, sizeof(struct PARSER_STRUCT));
  parser->lexer = lexer;
  parser->current_token = lexer_next(lexer);
  parser->prev_token = parser->current_token;

  parser->scope = init_scope();

  return parser;
}

ast_ *parser_parse(parser_ *parser, scope_ *scope)
{
  return parser_parse_statements(parser, scope);
}

void parser_expect(parser_ *parser, enum token_type expected_type)
{
  if (parser->current_token->type & expected_type)
  {
    parser->prev_token = parser->current_token;
    parser->current_token = lexer_next(parser->lexer);
  }
  else
  {
    printf(
        "Unexpected token `%s`, with type %d\n",
        parser->current_token->value,
        parser->current_token->type);
    exit(1);
  }
}

ast_ *parser_parse_statement(parser_ *parser, scope_ *scope)
{
  if (parser->current_token->type == TOKEN_ID)
    return parser_parse_id(parser, scope);
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

  while (parser->current_token->type != TOKEN_EOF)
  {
    if (parser->current_token->type == TOKEN_NEWLINE)
    {
      parser_expect(parser, TOKEN_NEWLINE);
    }

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
  // printf("\n");

  return compound;
}

ast_ *parser_parse_id(parser_ *parser, scope_ *scope)
{
  keyword_map_entry keyword_map[] = {
      {"CONSTANT", handle_constant},
      {"REPEAT", handle_repeat},
      {"WHILE", handle_while},
      {"FOR", handle_for},
      {"IF", handle_if},
      {"RECORD", handle_record},
      {"SUBROUTINE", handle_subroutine},
      {"RETURN", handle_return},
      {"OUTPUT", handle_output},
      {NULL, NULL} // Sentinel value to mark the end of the array
  };

  const char *token_value = parser->current_token->value;

  for (int i = 0; keyword_map[i].keyword != NULL; i++)
  {
    if (strcmp(token_value, keyword_map[i].keyword) == 0)
    {
      return keyword_map[i].handler(parser, scope); // Call the handler function and return its result
    }
  }

  return handle_undefined(parser, scope);
}
