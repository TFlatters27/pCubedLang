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
    if (parser->prev_token->type != TOKEN_NEWLINE)
      printf("\tTOKEN: %s{%s}\n", token_type_to_string(parser->prev_token->type), parser->prev_token->value);
  }
  else
  {
    printf(
        "Expected token `%s`{%s} at %d:%d\n",
        token_type_to_string(expected_type), parser->current_token->value,
        parser->lexer->line, parser->lexer->column);
    exit(1);
  }
}

ast_ *parser_parse_statement(parser_ *parser, scope_ *scope)
{
  if (parser->current_token->type == TOKEN_ID)
  {
    ast_ *ast = parser_parse_id(parser, scope);
    printf("Parsed AST expression %s\n", ast_type_to_string(ast->type));

    return ast;
  }
  else if (parser->current_token->type == TOKEN_NEWLINE)
  {
    parser_expect(parser, TOKEN_NEWLINE); // Consume newline after each statement
    return init_ast(AST_NOOP);
  }
  // exit(1);
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
      {"CONSTANT", handle_id},
      {"REPEAT", handle_undefined_loop},
      {"WHILE", handle_undefined_loop},
      {"FOR", handle_defined_loop},
      {"IF", handle_selection},
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

  return handle_id(parser, scope);
}

ast_ **init_ast_list()
{
  ast_ **list = malloc(sizeof(ast_ *));
  if (list == NULL)
  {
    fprintf(stderr, "Error: Memory allocation failed during AST list initialization.\n");
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
    fprintf(stderr, "Error: Cannot add AST to list. The list is NULL. Ensure the list is initialized before adding elements.\n");
    return;
  }

  while ((*list)[count] != NULL)
  {
    count++;
  }

  ast_ **temp = realloc(*list, sizeof(ast_ *) * (count + 2));
  if (temp == NULL)
  {
    fprintf(stderr, "Error: Memory reallocation failed while expanding AST list. Current list size: %zu elements.\n", count);
    return;
  }

  *list = temp;
  (*list)[count] = new_ast;
  (*list)[count + 1] = NULL;
}

// Helper function to handle unary expressions like "-" and "NOT"
ast_ *parse_unary_expression(parser_ *parser, scope_ *scope)
{
  printf("***** UNARY EXPRESSION *****\n");
  ast_ *expression = NULL;

  // Handling unary minus
  if (parser->current_token->type == TOKEN_ARITH_OP && strcmp(parser->current_token->value, "-") == 0)
  {
    parser_expect(parser, TOKEN_ARITH_OP); // Consume the unary minus
    ast_ *right_expression = parse_expression(parser, scope);

    // Check for integer, real, or ID types
    if (right_expression->type != AST_INTEGER && right_expression->type != AST_REAL && right_expression->type != AST_VARIABLE)
    {
      fprintf(stderr, "Type error: Unary minus can only be applied to INT, REAL, or ID\n");
      exit(1);
    }

    expression = init_ast(AST_ARITHMETIC_EXPRESSION);
    expression->op = strdup("-");
    expression->right = right_expression;
    return expression;
  }

  // Handling NOT operator
  if (parser->current_token->type == TOKEN_BOOL_OP && strcmp(parser->current_token->value, "NOT") == 0)
  {
    parser_expect(parser, TOKEN_BOOL_OP); // Consume the NOT operator
    ast_ *right_expression = parse_expression(parser, scope);

    // Check for boolean expressions or IDs
    if (right_expression->type != AST_BOOLEAN_EXPRESSION && right_expression->type != AST_VARIABLE)
    {
      fprintf(stderr, "Type error: NOT can only be applied to BOOLEAN expressions or ID\n");
      exit(1);
    }

    expression = init_ast(AST_BOOLEAN_EXPRESSION);
    expression->op = strdup("NOT");
    expression->right = right_expression;
    return expression;
  }

  return expression;
}

// Helper function to handle variables, array access, and record access
ast_ *parse_variable_or_access(parser_ *parser, scope_ *scope)
{
  printf("***** VARIABLE/ACCESS EXPRESSION *****\n");

  ast_ *expression = NULL;
  char *variable_name = NULL;
  int is_constant = 0;

  if (strcmp(parser->current_token->value, "CONSTANT") == 0)
  {
    is_constant = 1;                 // Mark as constant
    parser_expect(parser, TOKEN_ID); // Skip 'CONSTANT'
  }

  variable_name = parser->current_token->value;
  parser_expect(parser, TOKEN_ID); // Consume the identifier

  // Handle array access
  if (parser->current_token->type == TOKEN_LBRACKET)
  {
    expression = init_ast(AST_ARRAY_ACCESS);
    expression->variable_name = variable_name;
    expression->index = init_ast_list();
    while (parser->current_token->type == TOKEN_LBRACKET)
    {
      parser_expect(parser, TOKEN_LBRACKET);
      ast_ *element = parse_expression(parser, scope); // Parse the index
      add_ast_to_list(&(expression->index), element);
      parser_expect(parser, TOKEN_RBRACKET); // Consume ']'
    }
  }
  // Handle record access
  else if (parser->current_token->type == TOKEN_FULLSTOP)
  {
    expression = init_ast(AST_RECORD_ACCESS);
    parser_expect(parser, TOKEN_FULLSTOP); // Consume '.'
    expression->record_name = variable_name;
    expression->field_name = parser->current_token->value;
    parser_expect(parser, TOKEN_ID); // Consume field name
  }
  // Handle class instantiation
  else if (parser->current_token->type == TOKEN_LPAREN)
  {
    expression = init_ast(AST_INSTANTIATION);
    expression->class_name = variable_name;
    expression->arguments = init_ast_list();
    parser_expect(parser, TOKEN_LPAREN); // Consume '('
    if (parser->current_token->type != TOKEN_RPAREN)
    {
      do
      {
        ast_ *arg = parse_expression(parser, scope);
        add_ast_to_list(&(expression->arguments), arg);
        if (parser->current_token->type == TOKEN_COMMA)
        {
          parser_expect(parser, TOKEN_COMMA); // Consume ','
        }
        else
        {
          break;
        }
      } while (parser->current_token->type != TOKEN_RPAREN);
    }
    parser_expect(parser, TOKEN_RPAREN); // Consume ')'
  }
  // Handle simple variable or constant
  else
  {
    expression = init_ast(AST_VARIABLE);
    expression->variable_name = variable_name;
    expression->constant = is_constant;
  }

  return expression;
}

// Helper function to handle literals like integers, reals, characters, booleans, and strings
ast_ *parse_literal(parser_ *parser, scope_ *scope)
{
  printf("***** LITERAL EXPRESSION *****\n");

  ast_ *expression = NULL;

  if (parser->current_token->type == TOKEN_INT)
  {
    expression = init_ast(AST_INTEGER);
    expression->int_value = atoi(parser->current_token->value);
    parser_expect(parser, TOKEN_INT);
  }
  else if (parser->current_token->type == TOKEN_REAL)
  {
    expression = init_ast(AST_REAL);
    expression->real_value = atof(parser->current_token->value);
    parser_expect(parser, TOKEN_REAL);
  }
  else if (parser->current_token->type == TOKEN_CHAR)
  {
    expression = init_ast(AST_CHARACTER);
    expression->char_value = parser->current_token->value[0];
    parser_expect(parser, TOKEN_CHAR);
  }
  else if (parser->current_token->type == TOKEN_BOOL)
  {
    expression = init_ast(AST_BOOLEAN);
    expression->boolean_value = strcmp(parser->current_token->value, "True") == 0;
    parser_expect(parser, TOKEN_BOOL);
  }
  else if (parser->current_token->type == TOKEN_STRING)
  {
    expression = init_ast(AST_STRING);
    expression->string_value = strdup(parser->current_token->value);
    parser_expect(parser, TOKEN_STRING);
  }

  return expression;
}

// Helper function to parse arrays
ast_ *parse_array(parser_ *parser, scope_ *scope)
{
  ast_ *expression = init_ast(AST_ARRAY);
  expression->array_elements = init_ast_list();

  parser_expect(parser, TOKEN_LBRACKET); // Consume '['

  while (parser->current_token->type != TOKEN_RBRACKET)
  {
    ast_ *element = parse_expression(parser, scope);         // Parse each element
    add_ast_to_list(&(expression->array_elements), element); // Add element to the array

    if (parser->current_token->type == TOKEN_COMMA)
    {
      parser_expect(parser, TOKEN_COMMA); // Consume ','
    }
  }

  parser_expect(parser, TOKEN_RBRACKET); // Consume ']'
  return expression;
}

// Helper function to parse parentheses
ast_ *parse_parenthesized_expression(parser_ *parser, scope_ *scope)
{
  parser_expect(parser, TOKEN_LPAREN);                // Consume '('
  ast_ *expression = parse_expression(parser, scope); // Parse the inner expression
  parser_expect(parser, TOKEN_RPAREN);                // Consume ')'
  return expression;
}

// Helper function to handle binary arithmetic operations
ast_ *parse_binary_operation(parser_ *parser, ast_ *left, scope_ *scope)
{
  printf("***** BINARY EXPRESSION *****\n");

  ast_ *bin_op = init_ast(AST_ARITHMETIC_EXPRESSION);
  bin_op->op = strdup(parser->current_token->value);
  parser_expect(parser, TOKEN_ARITH_OP); // Consume the operator
  ast_ *right_expression = parse_expression(parser, scope);
  if (!left)
  {
    fprintf(stderr, "Type error: Invalid expression before binary operator '%s'\n", bin_op->op);
    exit(1);
  }
  bin_op->left = left;
  bin_op->right = right_expression;
  return bin_op;
}

// Helper function to handle relational operations
ast_ *parse_relational_operation(parser_ *parser, ast_ *left, scope_ *scope)
{
  printf("***** RELATIONAL EXPRESSION *****\n");

  ast_ *rel_op = init_ast(AST_BOOLEAN_EXPRESSION);
  rel_op->op = strdup(parser->current_token->value);
  parser_expect(parser, TOKEN_REL_OP); // Consume the operator
  ast_ *right_expression = parse_expression(parser, scope);
  if (!left)
  {
    fprintf(stderr, "Type error: Invalid expression before relational operator '%s'\n", rel_op->op);
    exit(1);
  }
  rel_op->left = left;
  rel_op->right = right_expression;
  return rel_op;
}

ast_ *parse_boolean_operation(parser_ *parser, ast_ *left, scope_ *scope)
{
  printf("***** RELATIONAL EXPRESSION *****\n");

  ast_ *bool_op = init_ast(AST_BOOLEAN_EXPRESSION);
  bool_op->op = strdup(parser->current_token->value);
  parser_expect(parser, TOKEN_BOOL_OP); // Consume the operator
  ast_ *right_expression = parse_expression(parser, scope);
  if (!left)
  {
    fprintf(stderr, "Type error: Invalid expression before relational operator '%s'\n", bool_op->op);
    exit(1);
  }
  bool_op->left = left;
  bool_op->right = right_expression;
  return bool_op;
}

// The main expression parsing function
ast_ *parse_expression(parser_ *parser, scope_ *scope)
{
  ast_ *expression = NULL;

  // Check for unary expressions
  expression = parse_unary_expression(parser, scope);
  if (expression)
    return expression;

  // Handle variables, arrays, records, or instantiation
  if (parser->current_token->type == TOKEN_ID)
  {
    expression = parse_variable_or_access(parser, scope);
  }
  // Handle literals
  else
  {
    expression = parse_literal(parser, scope);
  }

  // Handle arrays
  if (parser->current_token->type == TOKEN_LBRACKET)
  {
    expression = parse_array(parser, scope);
  }

  // Handle parentheses
  if (parser->current_token->type == TOKEN_LPAREN)
  {
    expression = parse_parenthesized_expression(parser, scope);
  }

  // Handle binary arithmetic operations
  if (parser->current_token->type == TOKEN_ARITH_OP)
  {
    expression = parse_binary_operation(parser, expression, scope);
  }
  // Handle relational operations
  else if (parser->current_token->type == TOKEN_REL_OP)
  {
    expression = parse_relational_operation(parser, expression, scope);
  }
  // Handle boolean operations
  else if (parser->current_token->type == TOKEN_BOOL_OP)
  {
    expression = parse_boolean_operation(parser, expression, scope);
  }

  return expression;
}

ast_ *handle_id(parser_ *parser, scope_ *scope)
{
  ast_ *lhs = NULL;

  // Parse the left-hand side (LHS) of the assignment, if present
  lhs = parse_expression(parser, scope);

  // Check if we hit the assignment operator (e.g., "<-")
  if (parser->current_token->type == TOKEN_ASSIGNMENT)
  {
    parser_expect(parser, TOKEN_ASSIGNMENT); // Expect and consume the assignment token

    // Parse the right-hand side (RHS) of the assignment
    ast_ *rhs = parse_expression(parser, scope);

    // Create an assignment node
    ast_ *assignment = init_ast(AST_ASSIGNMENT);
    assignment->lhs = lhs;
    assignment->rhs = rhs;

    return assignment; // Return the full assignment AST node
  }

  // If no assignment token is found, treat it as a standalone expression
  return lhs;
}

ast_ *handle_undefined_loop(parser_ *parser, scope_ *scope)
{
  // Initialize the AST node for the loop
  ast_ *loop_ast = init_ast(AST_INDEFINITE_LOOP);

  // Determine if it's a REPEAT...UNTIL loop or a WHILE...ENDWHILE loop
  if (strcmp(parser->current_token->value, "REPEAT") == 0)
  {
    parser_expect(parser, TOKEN_ID); // Consume 'REPEAT'

    parser_expect(parser, TOKEN_NEWLINE); // Consume the newline after the REPEAT statement

    // Initialize the loop body
    loop_ast->loop_body = init_ast_list();

    // Parse the loop body until we hit 'UNTIL'
    while (strcmp(parser->current_token->value, "UNTIL") != 0)
    {
      ast_ *statement = parser_parse_statement(parser, scope);
      add_ast_to_list(&loop_ast->loop_body, statement);
    }

    parser_expect(parser, TOKEN_ID); // Consume 'UNTIL'

    // Parse the condition expression after 'UNTIL'
    loop_ast->condition = parse_expression(parser, scope);
  }
  else if (strcmp(parser->current_token->value, "WHILE") == 0)
  {
    parser_expect(parser, TOKEN_ID); // Consume 'WHILE'

    // Parse the condition expression after 'WHILE'
    loop_ast->condition = parse_expression(parser, scope);

    parser_expect(parser, TOKEN_NEWLINE); // Consume the newline after the WHILE condition

    // Initialize the loop body
    loop_ast->loop_body = init_ast_list();

    // Parse the loop body until we hit 'ENDWHILE'
    while (strcmp(parser->current_token->value, "ENDWHILE") != 0)
    {
      ast_ *statement = parser_parse_statement(parser, scope);
      add_ast_to_list(&loop_ast->loop_body, statement);
    }

    parser_expect(parser, TOKEN_ID); // Consume 'ENDWHILE'
  }
  return loop_ast;
}

ast_ *handle_defined_loop(parser_ *parser, scope_ *scope)
{
  // Initialize the AST node for the loop
  ast_ *loop_ast = init_ast(AST_DEFINITE_LOOP);

  // Expect and consume the "FOR" keyword
  parser_expect(parser, TOKEN_ID); // Consume the 'FOR' token

  // Handle the loop variable
  loop_ast->loop_variable = init_ast(AST_VARIABLE);
  loop_ast->loop_variable->variable_name = parser->current_token->value;
  parser_expect(parser, TOKEN_ID); // Consume the loop variable

  // Determine if it's a "FOR variable IN collection" or "FOR variable <- start TO end"
  if (strcmp(parser->current_token->value, "<-") == 0)
  {
    // Case: FOR variable <- start TO end [STEP increment]

    parser_expect(parser, TOKEN_ASSIGNMENT); // Consume '<-'

    // Parse the start expression
    loop_ast->start_expr = parse_expression(parser, scope);

    // Expect and consume the "TO" keyword
    if (strcmp(parser->current_token->value, "TO") == 0)
    {
      parser_expect(parser, TOKEN_ID); // Consume 'TO'

      // Parse the end expression
      loop_ast->end_expr = parse_expression(parser, scope);
    }
    else
    {
      printf("Error: Expected 'TO' after start expression.\n");
      exit(1);
    }

    // Optional: Handle the STEP part
    if (strcmp(parser->current_token->value, "STEP") == 0)
    {
      parser_expect(parser, TOKEN_ID); // Consume 'STEP'
      loop_ast->step_expr = parse_expression(parser, scope);
    }
    else
    {
      // Default step value if not specified
      loop_ast->step_expr = init_ast(AST_INTEGER);
      loop_ast->step_expr->int_value = 1; // Default to step 1
    }
  }
  else if (strcmp(parser->current_token->value, "IN") == 0)
  {
    // Case: FOR variable IN collection

    parser_expect(parser, TOKEN_ID); // Consume 'IN'

    // Parse the collection expression (could be a string, array, etc.)
    loop_ast->collection_expr = parse_expression(parser, scope);
  }
  else
  {
    printf("Error: Expected '<-' or 'IN' after loop variable.\n");
    exit(1);
  }

  parser_expect(parser, TOKEN_NEWLINE); // Consume the newline after the FOR statement

  // Parse the loop body
  loop_ast->loop_body = init_ast_list(); // Initialize the loop body list

  while (strcmp(parser->current_token->value, "ENDFOR") != 0)
  {
    ast_ *statement = parser_parse_statement(parser, scope);
    add_ast_to_list(&(loop_ast->loop_body), statement); // Add each statement to the loop body
  }

  parser_expect(parser, TOKEN_ID); // Consume 'ENDFOR'

  return loop_ast;
}

ast_ *handle_selection(parser_ *parser, scope_ *scope)
{
  printf("Processing an IF block\n");

  // Initialize the AST node for the selection statement
  ast_ *selection_ast = init_ast(AST_SELECTION);

  // Expect and consume the "IF" keyword
  parser_expect(parser, TOKEN_ID); // Consume 'IF'

  // Parse the condition expression
  selection_ast->condition = parse_expression(parser, scope);

  // Expect and consume the "THEN" keyword
  parser_expect(parser, TOKEN_ID); // Consume 'THEN'

  // Parse the body of the IF statement
  selection_ast->if_body = init_ast_list();

  parser_expect(parser, TOKEN_NEWLINE); // Consume the newline after THEN
  while (parser->current_token->type != TOKEN_ID ||
         (strcmp(parser->current_token->value, "ENDIF") != 0 &&
          strcmp(parser->current_token->value, "ELSE") != 0))
  {
    ast_ *statement = parser_parse_statement(parser, scope);
    add_ast_to_list(&selection_ast->if_body, statement);
  }

  // Handle ELSE IF blocks
  ast_ **else_if_conditions = init_ast_list();
  ast_ **else_if_bodies = init_ast_list();

  while (strcmp(parser->current_token->value, "ELSE") == 0)
  {
    parser_expect(parser, TOKEN_ID); // Consume 'ELSE'

    if (strcmp(parser->current_token->value, "IF") == 0)
    {
      // Handle ELSE IF
      parser_expect(parser, TOKEN_ID); // Consume 'IF'

      // Parse the condition for ELSE IF
      ast_ *else_if_condition = parse_expression(parser, scope);
      add_ast_to_list(&else_if_conditions, else_if_condition);

      // Expect and consume the "THEN" keyword
      parser_expect(parser, TOKEN_ID); // Consume 'THEN'

      // Parse the body of the ELSE IF statement
      ast_ **else_if_body = init_ast_list();
      parser_expect(parser, TOKEN_NEWLINE); // Consume the newline after THEN
      while (parser->current_token->type != TOKEN_ID ||
             (strcmp(parser->current_token->value, "ENDIF") != 0 &&
              strcmp(parser->current_token->value, "ELSE") != 0))
      {
        ast_ *statement = parser_parse_statement(parser, scope);
        add_ast_to_list(&else_if_body, statement);
      }
      add_ast_to_list(&else_if_bodies, (ast_ *)else_if_body);
    }
    else
    {
      // Handle ELSE block
      selection_ast->else_body = init_ast_list();
      parser_expect(parser, TOKEN_NEWLINE); // Consume the newline after ELSE
      while (parser->current_token->type != TOKEN_ID || strcmp(parser->current_token->value, "ENDIF") != 0)
      {
        ast_ *statement = parser_parse_statement(parser, scope);
        add_ast_to_list(&selection_ast->else_body, statement);
      }
      break; // After ELSE, we expect ENDIF, so break out of the loop
    }
  }

  // Expect and consume the "ENDIF" keyword
  parser_expect(parser, TOKEN_ID); // Consume 'ENDIF'

  // Set the parsed ELSE IF conditions and bodies into the AST
  selection_ast->else_if_conditions = else_if_conditions;
  selection_ast->else_if_bodies = &else_if_bodies;

  return selection_ast;
}

ast_ *handle_record(parser_ *parser, scope_ *scope)
{
  // Expect and store the record's name
  parser_expect(parser, TOKEN_ID); // Consume 'RECORD'
  char *record_name = parser->current_token->value;
  parser_expect(parser, TOKEN_ID); // Consume the record name

  // Initialize the AST node for the record
  ast_ *record_ast = init_ast(AST_RECORD);
  record_ast->record_name = record_name;

  // Initialize the record elements list
  record_ast->record_elements = (ast_record_element_ **)init_ast_list();

  parser_expect(parser, TOKEN_NEWLINE);

  // Loop to parse all record fields
  while (strcmp(parser->current_token->value, "ENDRECORD") != 0)
  {
    // Handle the field name
    char *field_name = parser->current_token->value;
    parser_expect(parser, TOKEN_ID); // Consume the field name

    // Expect and handle the colon
    parser_expect(parser, TOKEN_COLON); // Consume ':'

    // Parse the field type
    ast_ *field_type_ast = NULL;

    if (strcmp(parser->current_token->value, "String") == 0)
    {
      field_type_ast = init_ast(AST_STRING);
    }
    else if (strcmp(parser->current_token->value, "Integer") == 0)
    {
      field_type_ast = init_ast(AST_INTEGER);
    }
    else if (strcmp(parser->current_token->value, "Real") == 0)
    {
      field_type_ast = init_ast(AST_REAL);
    }
    else if (strcmp(parser->current_token->value, "Boolean") == 0)
    {
      field_type_ast = init_ast(AST_BOOLEAN);
    }
    else if (strcmp(parser->current_token->value, "Char") == 0)
    {
      field_type_ast = init_ast(AST_CHARACTER);
    }
    else
    {
      printf("Unknown type: %s\n", parser->current_token->value);
      exit(1);
    }

    parser_expect(parser, TOKEN_ID); // Consume the type

    // Handle multi-dimensional arrays
    while (parser->current_token->type == TOKEN_LBRACKET)
    {
      int dimensions = 1;

      // Count the number of consecutive '[]' pairs
      while (parser->current_token->type == TOKEN_LBRACKET)
      {
        parser_expect(parser, TOKEN_LBRACKET); // Consume '['
        parser_expect(parser, TOKEN_RBRACKET); // Consume ']'
        dimensions++;
      }

      // Wrap the previous type in nested array types
      for (int i = 0; i < dimensions; i++)
      {
        ast_ *array_type_ast = init_ast(AST_ARRAY);
        array_type_ast->array_elements = (ast_ **)init_ast_list();
        add_ast_to_list(&array_type_ast->array_elements, field_type_ast);

        field_type_ast = array_type_ast;
      }
    }

    // Create the record element AST node
    ast_record_element_ *record_element = malloc(sizeof(ast_record_element_));
    record_element->element_name = field_name;
    record_element->type = field_type_ast;

    // Add the record element to the record's list
    add_ast_to_list((ast_ ***)&record_ast->record_elements, (ast_ *)record_element);

    parser_expect(parser, TOKEN_NEWLINE);
  }

  parser_expect(parser, TOKEN_ID); // Consume 'ENDRECORD'

  return record_ast;
}

ast_ *handle_subroutine(parser_ *parser, scope_ *scope)
{
  // Expect 'SUBROUTINE' keyword and the subroutine's name
  parser_expect(parser, TOKEN_ID); // Consume 'SUBROUTINE'
  char *subroutine_name = parser->current_token->value;
  parser_expect(parser, TOKEN_ID); // Consume the subroutine name

  // Create the AST node for the subroutine
  ast_ *subroutine_ast = init_ast(AST_SUBROUTINE);
  subroutine_ast->subroutine_name = subroutine_name;

  // Parse the parameter list
  parser_expect(parser, TOKEN_LPAREN); // Consume '('
  subroutine_ast->parameters = init_ast_list();

  while (parser->current_token->type != TOKEN_RPAREN)
  {
    if (parser->current_token->type == TOKEN_ID)
    {
      ast_ *param = init_ast(AST_VARIABLE);
      param->variable_name = parser->current_token->value;
      add_ast_to_list(&(subroutine_ast->parameters), param);
      parser_expect(parser, TOKEN_ID); // Consume the parameter name

      if (parser->current_token->type == TOKEN_COMMA)
      {
        parser_expect(parser, TOKEN_COMMA); // Consume ','
      }
    }
    else
    {
      printf("Error: Unexpected token in parameter list: %s\n", parser->current_token->value);
      exit(1);
    }
  }

  parser_expect(parser, TOKEN_RPAREN);  // Consume ')'
  parser_expect(parser, TOKEN_NEWLINE); // Consume newline

  // Parse the subroutine body
  subroutine_ast->body = init_ast_list();

  while (strcmp(parser->current_token->value, "ENDSUBROUTINE") != 0)
  {
    ast_ *statement = parser_parse_statement(parser, scope);

    // Check if the statement is a RETURN
    if (statement->type == AST_RETURN)
    {
      subroutine_ast->return_value = statement->return_value;
    }
    else
    {
      add_ast_to_list(&(subroutine_ast->body), statement);
    }
  }

  parser_expect(parser, TOKEN_ID); // Consume 'ENDSUBROUTINE'

  return subroutine_ast;
}

ast_ *handle_return(parser_ *parser, scope_ *scope)
{
  // Create an AST node for the RETURN statement
  ast_ *return_ast = init_ast(AST_RETURN);

  // Expect and consume the "RETURN" keyword
  parser_expect(parser, TOKEN_ID); // Consume the 'RETURN' token

  // Parse the expression following the RETURN keyword
  return_ast->return_value = parse_expression(parser, scope);

  return return_ast;
}

ast_ *handle_output(parser_ *parser, scope_ *scope)
{
  // Create an AST node for the OUTPUT statement
  ast_ *output_ast = init_ast(AST_OUTPUT);

  // Expect and consume the "OUTPUT" keyword
  parser_expect(parser, TOKEN_ID); // Consume the 'OUTPUT' token

  // Initialize the list of output expressions
  output_ast->output_expressions = NULL;
  output_ast->output_expressions_size = 0;

  // Loop to parse all expressions following the OUTPUT keyword
  do
  {
    // Parse each expression and add it to the output expressions list
    ast_ *expression = parse_expression(parser, scope);
    output_ast->output_expressions_size++;
    output_ast->output_expressions = realloc(output_ast->output_expressions,
                                             output_ast->output_expressions_size * sizeof(ast_ *));
    output_ast->output_expressions[output_ast->output_expressions_size - 1] = expression;

    // If the current token is a comma, consume it and continue parsing the next expression
    if (parser->current_token->type == TOKEN_COMMA)
    {
      parser_expect(parser, TOKEN_COMMA); // Consume the comma
    }
  } while (parser->current_token->type != TOKEN_NEWLINE && parser->current_token->type != TOKEN_EOF);

  return output_ast;
}
