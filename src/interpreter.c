#include "include/interpreter.h"
#include "include/scope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
ast_ *concatenate(ast_ *left_val, ast_ *right_val)
{
  char *result = NULL;

  // Handle character + character
  if (left_val->type == AST_CHARACTER && right_val->type == AST_CHARACTER)
  {
    result = (char *)malloc(3); // 2 chars + null terminator
    if (result == NULL)
    {
      perror("Failed to allocate memory for char concatenation");
      exit(1);
    }
    result[0] = left_val->char_value;
    result[1] = right_val->char_value;
    result[2] = '\0';
  }
  // Handle string + character
  else if (left_val->type == AST_STRING && right_val->type == AST_CHARACTER)
  {
    result = (char *)malloc(strlen(left_val->string_value) + 2); // 1 char + null terminator
    if (result == NULL)
    {
      perror("Failed to allocate memory for string-char concatenation");
      exit(1);
    }
    strcpy(result, left_val->string_value);
    result[strlen(left_val->string_value)] = right_val->char_value;
    result[strlen(left_val->string_value) + 1] = '\0';
  }
  // Handle character + string
  else if (left_val->type == AST_CHARACTER && right_val->type == AST_STRING)
  {
    result = (char *)malloc(2 + strlen(right_val->string_value)); // 1 char + string + null terminator
    if (result == NULL)
    {
      perror("Failed to allocate memory for char-string concatenation");
      exit(1);
    }
    result[0] = left_val->char_value;
    strcpy(&result[1], right_val->string_value);
  }
  // Handle string + string
  else if (left_val->type == AST_STRING && right_val->type == AST_STRING)
  {
    result = (char *)malloc(strlen(left_val->string_value) + strlen(right_val->string_value) + 1);
    if (result == NULL)
    {
      perror("Failed to allocate memory for string-string concatenation");
      exit(1);
    }
    strcpy(result, left_val->string_value);
    strcat(result, right_val->string_value);
  }
  // Invalid types for concatenation
  else
  {
    perror("Concatenation error: Unsupported types");
    exit(1);
  }

  // Create a new AST node for the concatenated result
  ast_ *new_node = init_ast(AST_STRING);
  new_node->string_value = result;

  return new_node;
}

interpreter_ *init_interpreter()
{
  interpreter_ *interpreter = calloc(1, sizeof(struct INTERPRETER_STRUCT));
  return interpreter;
}

ast_ *interpreter_process(interpreter_ *interpreter, ast_ *node)
{
  switch (node->type)
  {
  case AST_COMPOUND:
    return interpreter_process_compound(interpreter, node);
  case AST_NOOP:
    return node;
  case AST_INTEGER:
    return interpreter_process_integer(interpreter, node);
  case AST_REAL:
    return interpreter_process_real(interpreter, node);
  case AST_CHARACTER:
    return interpreter_process_character(interpreter, node);
  case AST_STRING:
    return interpreter_process_string(interpreter, node);
  case AST_BOOLEAN:
    return interpreter_process_boolean(interpreter, node);
  case AST_ARRAY:
    return interpreter_process_array(interpreter, node);
  case AST_ASSIGNMENT:
    return interpreter_process_assignment(interpreter, node);
  case AST_VARIABLE:
    return interpreter_process_variable(interpreter, node);
  case AST_RECORD_ACCESS:
    return interpreter_process_record_access(interpreter, node);
  case AST_ARRAY_ACCESS:
    return interpreter_process_array_access(interpreter, node);
  case AST_INSTANTIATION:
    return interpreter_process_instantiation(interpreter, node);
  case AST_ARITHMETIC_EXPRESSION:
    return interpreter_process_arithmetic_expression(interpreter, node);
  case AST_BOOLEAN_EXPRESSION:
    return interpreter_process_boolean_expression(interpreter, node);
  case AST_RECORD:
    return interpreter_process_record(interpreter, node);
  case AST_SUBROUTINE:
    return interpreter_process_subroutine(interpreter, node);
  case AST_RETURN:
    return interpreter_process_return(interpreter, node);
  case AST_OUTPUT:
    return interpreter_process_output(interpreter, node);
  case AST_DEFINITE_LOOP:
    return interpreter_process_definite_loop(interpreter, node);
  case AST_INDEFINITE_LOOP:
    return interpreter_process_indefinite_loop(interpreter, node);
  case AST_SELECTION:
    return interpreter_process_selection(interpreter, node);
  default:
    fprintf(stderr, "Uncaught statement of type `%s`\n", ast_type_to_string(node->type));
    exit(1);
  }

  return init_ast(AST_NOOP); // This line will never be reached due to the exit above
}

ast_ *interpreter_process_compound(interpreter_ *interpreter, ast_ *node)
{
  // printf(">> compound <<\n");
  int i = 0;
  while (node->compound_value[i] != NULL)
  {
    // printf("%d\n", i);
    interpreter_process(interpreter, node->compound_value[i]);
    i++;
  }
  return init_ast(AST_NOOP);
}
ast_ *interpreter_process_noop(interpreter_ *interpreter, ast_ *node)
{
  printf(">> noop <<\n");
  return init_ast(AST_NOOP);
}
ast_ *interpreter_process_integer(interpreter_ *interpreter, ast_ *node)
{
  return node;
}
ast_ *interpreter_process_real(interpreter_ *interpreter, ast_ *node)
{
  return node;
}
ast_ *interpreter_process_character(interpreter_ *interpreter, ast_ *node)
{
  return node;
}
ast_ *interpreter_process_string(interpreter_ *interpreter, ast_ *node)
{
  return node;
}
ast_ *interpreter_process_boolean(interpreter_ *interpreter, ast_ *node)
{
  return node;
}
ast_ *interpreter_process_array(interpreter_ *interpreter, ast_ *node)
{
  return node;
}
ast_ *interpreter_process_assignment(interpreter_ *interpreter, ast_ *node)
{
  // printf(">> assignment <<\n");

  // Process right-hand side before storing in scope
  ast_ *rhs_value = interpreter_process(interpreter, node->rhs);
  node->rhs = rhs_value; // Update the right-hand side with the processed value

  // Add the variable to the scope
  scope_add_variable_definition(node->scope, node);

  return node;
}

ast_ *interpreter_process_variable(interpreter_ *interpreter, ast_ *node)
{
  // printf(">> variable <<\n");

  // Fetch variable definition from scope
  ast_ *vdef = scope_get_variable_definition(node->scope, node->variable_name);

  if (vdef != NULL)
  {
    // Process and return the left-hand side (i.e., the variable's value)
    return interpreter_process(interpreter, vdef);
  }

  // If the variable is undefined
  printf("Undefined variable `%s`\n", node->variable_name);
  exit(1);
  return init_ast(AST_NOOP); // Return noop as fallback
}

ast_ *interpreter_process_record_access(interpreter_ *interpreter, ast_ *node)
{
  printf(">> record_access <<\n");
  return init_ast(AST_NOOP); // Return noop as fallback
}

ast_ *interpreter_process_array_access(interpreter_ *interpreter, ast_ *node)
{
  // Retrieve the array from the current scope
  ast_ *array = scope_get_variable_definition(node->scope, node->variable_name);

  if (!array)
  {
    fprintf(stderr, "Error: could not fetch array from scope\n");
    return init_ast(AST_NOOP);
  }

  if (array->type != AST_ARRAY)
  {
    fprintf(stderr, "Error: %s is not defined as an array.\n", node->variable_name);
    return init_ast(AST_NOOP);
  }

  ast_ *current_array = array;
  int b = 0;

  // Iterate over the indices to access nested arrays
  while (node->index[b] != NULL)
  {
    ast_ *index_value = interpreter_process(interpreter, node->index[b]);

    if (index_value->type != AST_INTEGER)
    {
      printf("Error: Array index must be an integer.\n");
      return init_ast(AST_NOOP);
    }

    int index = index_value->int_value;

    // Ensure that current_array is an array and the index is within bounds
    if (current_array->type != AST_ARRAY || index < 0 || index >= current_array->array_size)
    {
      printf("Error: Array index out of bounds or invalid array access.\n");
      return init_ast(AST_NOOP);
    }

    // Move to the next nested array or value
    current_array = current_array->array_elements[index];
    b++;
  }

  return current_array;
}

ast_ *interpreter_process_instantiation(interpreter_ *interpreter, ast_ *node)
{
  printf(">> instantiation <<\n");
  return init_ast(AST_NOOP);
}
ast_ *interpreter_process_arithmetic_expression(interpreter_ *interpreter, ast_ *node)
{
  // printf(">> arithmetic_expression <<\n");

  ast_ *left_val = NULL;
  ast_ *right_val = NULL;

  // Process the left side, if not null
  if (node->left != NULL)
  {
    left_val = interpreter_process(interpreter, node->left);
  }

  // Process the right side, which should always exist
  right_val = interpreter_process(interpreter, node->right);

  // Handle unary minus operation
  if ((strcmp(node->op, "-") == 0) && node->left == NULL)
  {
    if (right_val->type == AST_INTEGER)
    {
      right_val->int_value = -(right_val->int_value);
      return right_val;
    }
    else if (right_val->type == AST_REAL)
    {
      right_val->real_value = -(right_val->real_value);
      return right_val;
    }
  }

  // Handle binary operations
  if (strcmp(node->op, "+") == 0)
  {
    if ((left_val->type == AST_CHARACTER || left_val->type == AST_STRING) &&
        (right_val->type == AST_CHARACTER || right_val->type == AST_STRING))
    {
      return concatenate(left_val, right_val); // General concatenation for char/string
    }

    else if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      left_val->int_value += right_val->int_value;
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      left_val->real_value += right_val->real_value;
      return left_val;
    }
  }
  else if (strcmp(node->op, "-") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      left_val->int_value -= right_val->int_value;
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      left_val->real_value -= right_val->real_value;
      return left_val;
    }
  }
  else if (strcmp(node->op, "*") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      left_val->int_value *= right_val->int_value;
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      left_val->real_value *= right_val->real_value;
      return left_val;
    }
  }
  else if (strcmp(node->op, "/") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      left_val->int_value /= right_val->int_value;
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      left_val->real_value /= right_val->real_value;
      return left_val;
    }
  }
  else if (strcmp(node->op, "^") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      left_val->int_value = pow(left_val->int_value, right_val->int_value);
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      left_val->real_value = pow(left_val->real_value, right_val->real_value);
      return left_val;
    }
  }
  else if (strcmp(node->op, "DIV") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      left_val->int_value /= right_val->int_value; // Integer division
      return left_val;
    }
  }
  else if (strcmp(node->op, "MOD") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      left_val->int_value %= right_val->int_value;
      return left_val;
    }
  }

  printf("Invalid arithmetic operation: %s\n", node->op);
  exit(1);
  return init_ast(AST_NOOP); // Return a no-op if no valid operation found
}

ast_ *interpreter_process_boolean_expression(interpreter_ *interpreter, ast_ *node)
{
  printf(">> boolean_expression <<\n");
  return init_ast(AST_NOOP);
}
ast_ *interpreter_process_record(interpreter_ *interpreter, ast_ *node)
{
  printf(">> record <<\n");
  scope_add_record_definition(
      node->scope,
      node);

  return node;
}
ast_ *interpreter_process_subroutine(interpreter_ *interpreter, ast_ *node)
{
  printf(">> subroutine <<\n");
  scope_add_subroutine_definition(
      node->scope,
      node);

  return node;
}
ast_ *interpreter_process_return(interpreter_ *interpreter, ast_ *node)
{
  printf(">> return <<\n");

  // Process the return value
  ast_ *return_value = interpreter_process(interpreter, node->return_value);

  return return_value;
}

ast_ *interpreter_process_output(interpreter_ *interpreter, ast_ *node)
{
  // Loop through each output expression
  for (size_t i = 0; node->output_expressions[i] != NULL; i++)
  {
    ast_ *expr = interpreter_process(interpreter, node->output_expressions[i]);

    switch (expr->type)
    {
    case AST_STRING:
      printf("%s", expr->string_value);
      break;

    case AST_INTEGER:
      printf("%d", expr->int_value);
      break;

    case AST_REAL:
      printf("%0.2f", expr->real_value);
      break;

    case AST_CHARACTER:
      printf("%c", expr->char_value);
      break;

    case AST_BOOLEAN:
      printf("%s", expr->boolean_value ? "True" : "False");
      break;

    default:
      fprintf(stderr, "Unsupported output type in AST_OUTPUT.\n");
      exit(1);
    }

    // Print a space between expressions, but avoid trailing space after the last expression
    if (node->output_expressions[i + 1] != NULL)
    {
      printf(" ");
    }
  }

  // After printing all expressions, add a newline
  printf("\n");

  return init_ast(AST_NOOP);
}

ast_ *interpreter_process_definite_loop(interpreter_ *interpreter, ast_ *node)
{
  printf(">> definite_loop <<\n");

  return init_ast(AST_NOOP);
}

ast_ *interpreter_process_indefinite_loop(interpreter_ *interpreter, ast_ *node)
{
  printf(">> indefinite_loop <<\n");
  return init_ast(AST_NOOP);
}
ast_ *interpreter_process_selection(interpreter_ *interpreter, ast_ *node)
{
  printf(">> selection <<\n");
  return init_ast(AST_NOOP);
}
