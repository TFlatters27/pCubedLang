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
    printf("\t\tconsumed TOKEN: %s{%s}\n", token_type_to_string(parser->prev_token->type), parser->prev_token->value);
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
    printf("PARSED STATEMENT: %s\n", ast_type_to_string(ast->type));
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
      {"CONSTANT", handle_assignment},
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

  return handle_assignment(parser, scope);
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

ast_ *parse_expression(parser_ *parser, scope_ *scope)
{
  ast_ *expression = NULL;
  token_ *current_token = parser->current_token;

  // Handle potential unary operators at the start
  if (current_token->type == TOKEN_BIN_OP && strcmp(current_token->value, "-") == 0)
  {
    // This is a unary minus
    parser_expect(parser, TOKEN_BIN_OP);
    expression = init_ast(AST_UNARY_OP);
    expression->unary_op = strdup(current_token->value);   // Save the operator (in this case, "-")
    expression->operand = parse_expression(parser, scope); // Parse the right-hand side expression
    return expression;
  }

  // Handle "NOT" operator (Unary Boolean operation)
  if (current_token->type == TOKEN_REL_OP && strcmp(current_token->value, "NOT") == 0)
  {
    // This is a unary boolean negation (NOT)
    parser_expect(parser, TOKEN_REL_OP);
    expression = init_ast(AST_UNARY_OP);
    expression->unary_op = strdup(current_token->value);   // Save the operator (in this case, "NOT")
    expression->operand = parse_expression(parser, scope); // Parse the right-hand side expression
    return expression;
  }

  // Parse primary expressions first
  if (current_token->type & (TOKEN_INT | TOKEN_REAL))
  {
    if (current_token->type == TOKEN_INT)
    {
      expression = init_ast(AST_INT);
      expression->int_value = atoi(current_token->value);
    }
    else
    {
      expression = init_ast(AST_REAL);
      expression->real_value = atof(current_token->value);
    }
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
      expression = init_ast(AST_INSTANTIATION);
      expression->instantiated_type = variable_id;
      expression->arguments = init_ast_list();
      parser_expect(parser, TOKEN_LPAREN);

      while (parser->current_token->type != TOKEN_RPAREN)
      {
        ast_ *arg = parse_expression(parser, scope);
        add_ast_to_list(&(expression->arguments), arg);

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
  else
  {
    printf("Unexpected token in expression: %s\n", current_token->value);
    exit(1);
  }

  // Handle binary operations (including AND, OR)
  while (parser->current_token->type == TOKEN_BIN_OP || parser->current_token->type == TOKEN_REL_OP)
  {
    token_ *op_token = parser->current_token;
    char *op_value = op_token->value;

    parser_expect(parser, (TOKEN_BIN_OP | TOKEN_REL_OP));

    ast_ *right = parse_expression(parser, scope);

    // Create the binary operation AST node
    ast_ *bin_op = NULL;

    if (strcmp(op_value, "AND") == 0 || strcmp(op_value, "OR") == 0)
    {
      bin_op = init_ast(AST_BINARY_OP);
      bin_op->binary_op = op_value;
    }
    else
    {
      bin_op = init_ast(AST_RELATIONAL_OP);
      bin_op->binary_op = op_value;
    }

    bin_op->left = expression;
    bin_op->right = right;
    expression = bin_op;
  }

  // Handle relational operations (as previously)
  while (parser->current_token->type == TOKEN_REL_OP)
  {
    token_ *op_token = parser->current_token;
    char *op_value = op_token->value;

    parser_expect(parser, TOKEN_REL_OP);

    ast_ *right = parse_expression(parser, scope);

    // Create the binary operation AST node
    ast_ *bin_op = init_ast(AST_RELATIONAL_OP);
    bin_op->binary_op = op_value;

    bin_op->left = expression;
    bin_op->right = right;
    expression = bin_op;
  }

  return expression;
}

ast_ *handle_assignment(parser_ *parser, scope_ *scope)
{
  ast_ *lhs = NULL;
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

  // Start building the LHS
  lhs = init_ast(AST_VARIABLE);
  lhs->variable_name = variable_id;

  // Handle array indexing or record field access
  while (parser->current_token->type == TOKEN_LBRACKET || parser->current_token->type == TOKEN_FULLSTOP)
  {
    if (parser->current_token->type == TOKEN_LBRACKET)
    {
      // Array access
      parser_expect(parser, TOKEN_LBRACKET);
      ast_ *index_expr = parse_expression(parser, scope);
      parser_expect(parser, TOKEN_RBRACKET);

      ast_ *array_access = init_ast(AST_ARRAY);
      array_access->left = lhs;
      array_access->elements = malloc(sizeof(ast_ *));
      array_access->elements[0] = index_expr;
      array_access->elements_size = 1;

      lhs = array_access; // Update lhs to reflect array access
    }
    else if (parser->current_token->type == TOKEN_FULLSTOP)
    {
      // Record field access
      parser_expect(parser, TOKEN_FULLSTOP);
      char *field_name = parser->current_token->value;
      parser_expect(parser, TOKEN_ID);

      ast_ *record_access = init_ast(AST_RECORD_ACCESS);
      record_access->left = lhs;
      record_access->variable_name = field_name;

      lhs = record_access; // Update lhs to reflect field access
    }
  }

  // Expect the assignment operator
  parser_expect(parser, TOKEN_ASSIGNMENT);

  // Parse the right-hand side expression
  ast_ *rhs = parse_expression(parser, scope);

  // Create the assignment node
  ast_ *assignment = init_ast(AST_ASSIGNMENT);
  assignment->lhs = lhs;                // Set the left-hand side of the assignment
  assignment->rhs = rhs;                // Set the right-hand side of the assignment
  assignment->constant = constant_flag; // Set the constant flag

  return assignment;
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
  else
  {
    printf("Error: Expected 'REPEAT' or 'WHILE' to start an undefined loop.\n");
    exit(1);
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
      loop_ast->step_expr = init_ast(AST_INT);
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
      field_type_ast = init_ast(AST_INT);
    }
    else if (strcmp(parser->current_token->value, "Real") == 0)
    {
      field_type_ast = init_ast(AST_REAL);
    }
    else if (strcmp(parser->current_token->value, "Boolean") == 0)
    {
      field_type_ast = init_ast(AST_BOOL);
    }
    else if (strcmp(parser->current_token->value, "Char") == 0)
    {
      field_type_ast = init_ast(AST_CHAR);
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
        array_type_ast->elements_size = 0; // Placeholder for array elements if needed
        array_type_ast->elements = (ast_ **)init_ast_list();
        add_ast_to_list(&array_type_ast->elements, field_type_ast);

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
  subroutine_ast->parameters_size = 0;
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
  subroutine_ast->body_size = 0;
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
