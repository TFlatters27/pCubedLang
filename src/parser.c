#include "include/parser.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

ast_ **init_ast_list()
{
  ast_ **list = malloc(sizeof(ast_ *));
  if (list == NULL)
  {
    printf("Memory allocation failed in list initialisation\n");
    return NULL;
  }
  list[0] = NULL;
  return list;
}

void add_ast_to_list(ast_ ***list, ast_ *new_ast)
{
  size_t count = 0;

  if (*list == NULL)
  {
    printf("List is NULL\n");
    return;
  }

  while ((*list)[count] != NULL)
  {
    count++;
  }

  ast_ **temp = realloc(*list, sizeof(ast_ *) * (count + 2));
  if (temp == NULL)
  {
    printf("Memory allocation failed\n");
    return;
  }

  *list = temp;

  (*list)[count] = new_ast;

  (*list)[count + 1] = NULL;
}

ast_ *parse_expression(parser_ *parser, scope_ *scope)
{
  ast_ *expression = NULL;
  token_ *current_token = parser->current_token;

  if (current_token->type & (TOKEN_INT | TOKEN_REAL))
  {
    expression = init_ast(AST_NUM);
    expression->int_value = atoi(current_token->value);
    parser_expect(parser, TOKEN_INT | TOKEN_REAL);
  }
  else if (current_token->type == TOKEN_STRING)
  {
    expression = init_ast(AST_STRING);
    expression->string_value = current_token->value;
    parser_expect(parser, TOKEN_STRING);
  }
  else if (current_token->type == TOKEN_CHAR)
  {
    expression = init_ast(AST_CHAR);
    expression->char_value = current_token->value[0];
    parser_expect(parser, TOKEN_CHAR);
  }
  else if (current_token->type == TOKEN_BOOL)
  {
    expression = init_ast(AST_BOOL);
    expression->bool_value = (strcmp(current_token->value, "True") == 0);
    parser_expect(parser, TOKEN_BOOL);
  }
  else if (current_token->type == TOKEN_ID)
  {
    char *variable_id = current_token->value;
    parser_expect(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_LPAREN)
    {
      expression = init_ast(AST_FUNCTION_CALL);
      expression->instantiated_type = variable_id;
      expression->arguments = init_ast_list();
      parser_expect(parser, TOKEN_LPAREN);

      while (parser->current_token->type != TOKEN_RPAREN)
      {
        ast_ *arg = parse_expression(parser, scope);
        add_ast_to_list(expression->arguments, arg);

        if (parser->current_token->type == TOKEN_COMMA)
        {
          parser_expect(parser, TOKEN_COMMA);
        }
      }

      parser_expect(parser, TOKEN_RPAREN);
    }
    else
    {
      expression = init_ast(AST_VARIABLE);
      expression->variable_name = variable_id;
    }
  }
  else if (current_token->type == TOKEN_LBRACKET)
  {
    expression = init_ast(AST_ARRAY);
    expression->elements = init_ast_list();
    parser_expect(parser, TOKEN_LBRACKET);

    while (parser->current_token->type != TOKEN_RBRACKET)
    {
      ast_ *element = parse_expression(parser, scope);
      add_ast_to_list(&(expression->elements), element);

      if (parser->current_token->type == TOKEN_COMMA)
      {
        parser_expect(parser, TOKEN_COMMA);
      }
    }

    parser_expect(parser, TOKEN_RBRACKET);
  }
  else if (current_token->type == TOKEN_LPAREN)
  {
    parser_expect(parser, TOKEN_LPAREN);
    expression = parse_expression(parser, scope);
    parser_expect(parser, TOKEN_RPAREN);
  }
  else if (current_token->type == TOKEN_UN_OP)
  {
    expression = init_ast(AST_UNARY_OP);
    expression->unary_op = current_token->type;
    parser_expect(parser, current_token->type);
    expression->operand = parse_expression(parser, scope);
  }
  else if (current_token->type == TOKEN_BIN_OP)
  {
    ast_ *left = parse_expression(parser, scope);
    while (parser->current_token->type == TOKEN_BIN_OP)
    {
      ast_ *bin_op = init_ast(AST_BINARY_OP);
      bin_op->left = left;
      bin_op->binary_op = parser->current_token->type;
      parser_expect(parser, parser->current_token->type);
      bin_op->right = parse_expression(parser, scope);
      left = bin_op;
    }
    expression = left;
  }
  else
  {
    printf("Unexpected token in expression: %s\n", current_token->value);
    exit(1);
  }

  return expression;
}

ast_ *handle_variable_assignment(parser_ *parser, scope_ *scope)
{
  char *variable_id = parser->current_token->value;
  parser_expect(parser, TOKEN_ID);

  int constant_flag = 0;

  // Check if the ID is "CONSTANT"
  if (strcmp(variable_id, "CONSTANT") == 0)
  {
    constant_flag = 1;

    // Expect the next token to be another ID
    variable_id = parser->current_token->value;
    parser_expect(parser, TOKEN_ID);
  }

  parser_expect(parser, TOKEN_ASSIGNMENT);

  if (parser->current_token->type == TOKEN_ID)
  {
    ast_ *instantiation = init_ast(AST_INSTANTIATION);
    instantiation->variable_name = variable_id;

    instantiation->instantiated_type = parser->current_token->value;
    parser_expect(parser, TOKEN_ID);

    parser_expect(parser, TOKEN_LPAREN);

    instantiation->arguments = init_ast_list();

    while (parser->current_token->type != TOKEN_RPAREN)
    {
      ast_ *arg = parse_expression(parser, scope);
      add_ast_to_list(instantiation->arguments, arg);

      if (parser->current_token->type == TOKEN_COMMA)
      {
        parser_expect(parser, TOKEN_COMMA); // Consume the comma
      }
    }

    parser_expect(parser, TOKEN_RPAREN);

    return instantiation;
  }
  else
  {
    ast_ *assignment = init_ast(AST_ASSIGNMENT);
    assignment->variable_name = variable_id;

    assignment->value = parse_expression(parser, scope);

    // Set the constant flag in the assignment node
    assignment->constant = constant_flag;

    return assignment;
  }
}

ast_ *handle_array_assignment(parser_ *parser, scope_ *scope)
{
  printf("Processing array assignment\n");
  exit(0);
  return init_ast(AST_NOOP);
}
ast_ *handle_record_assignment(parser_ *parser, scope_ *scope)
{
  printf("Processing record assignment\n");

  exit(0);
  return init_ast(AST_NOOP);
}
ast_ *handle_record_field(parser_ *parser, scope_ *scope)
{
  printf("Processing record field\n");
  exit(0);
  return init_ast(AST_NOOP);
}
ast_ *handle_subroutine_call(parser_ *parser, scope_ *scope)
{
  printf("Processing subroutine call statement\n");
  exit(0);
  return init_ast(AST_NOOP);
}

ast_ *handle_undefined(parser_ *parser, scope_ *scope)
{
  token_ *preceeding_token = lexer_peek(parser->lexer);
  switch (preceeding_token->type)
  {
  case TOKEN_ASSIGNMENT:
    // id <- expression
    return handle_variable_assignment(parser, scope);
  case TOKEN_LBRACKET:
    // id[intExpression]... <- expression
    return handle_array_assignment(parser, scope);
  case TOKEN_FULLSTOP:
    // id.id... <- expression
    return handle_record_assignment(parser, scope);
  case TOKEN_COLON:
    // id : id{data type}
    return handle_record_field(parser, scope);
  case TOKEN_LPAREN:
    // id(val...)
    return handle_subroutine_call(parser, scope);
  }
}

ast_ *handle_undefined_loop(parser_ *parser, scope_ *scope)
{
  printf("Processing an undefined loop block\n");
  exit(0);
  return init_ast(AST_NOOP);
}

ast_ *handle_defined_loop(parser_ *parser, scope_ *scope)
{
  printf("Processing a defined loop block\n");
  exit(0);
  return init_ast(AST_NOOP);
}

ast_ *handle_if(parser_ *parser, scope_ *scope)
{
  printf("Processing an IF block\n");
  exit(0);
  return init_ast(AST_NOOP);
}

ast_ *handle_record(parser_ *parser, scope_ *scope)
{
  printf("Processing a RECORD block\n");
  exit(0);
  return init_ast(AST_NOOP);
}

ast_ *handle_subroutine(parser_ *parser, scope_ *scope)
{
  printf("Processing a SUBROUTINE block\n");
  exit(0);
  return init_ast(AST_NOOP);
}

ast_ *handle_return(parser_ *parser, scope_ *scope)
{
  printf("Processing a RETURN block\n");
  exit(0);
  return init_ast(AST_NOOP);
}

ast_ *handle_output(parser_ *parser, scope_ *scope)
{
  printf("Processing an OUTPUT block\n");
  exit(0);
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
    printf("\tconsumed TOKEN: %s\n", token_type_to_string(parser->prev_token->type));
  }
  else
  {
    printf(
        "Expected token `%s` at %d:%d\n",
        token_type_to_string(expected_type),
        parser->lexer->line, parser->lexer->column);
    exit(1);
  }
}

ast_ *parser_parse_statement(parser_ *parser, scope_ *scope)
{
  if (parser->current_token->type == TOKEN_ID)
  {
    ast_ *ast = parser_parse_id(parser, scope);
    printf("PARSED STATEMENT: %s\n", ast_type_to_string(ast->type));
    return ast;
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

  return compound;
}

ast_ *parser_parse_id(parser_ *parser, scope_ *scope)
{
  keyword_map_entry keyword_map[] = {
      {"CONSTANT", handle_variable_assignment},
      {"REPEAT", handle_undefined_loop},
      {"WHILE", handle_undefined_loop},
      {"FOR", handle_defined_loop},
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
