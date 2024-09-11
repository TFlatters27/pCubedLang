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
    if (left_val->char_value.null == 0 && right_val->char_value.null == 0)
    {
      result = (char *)malloc(3); // 2 chars + null terminator
      if (result == NULL)
      {
        perror("Failed to allocate memory for char concatenation");
        exit(1);
      }
      result[0] = left_val->char_value.value;  // Access the char value
      result[1] = right_val->char_value.value; // Access the char value
      result[2] = '\0';
    }
    else
    {
      perror("Concatenation error: One of the character values is null");
      exit(1);
    }
  }
  // Handle string + character
  else if (left_val->type == AST_STRING && right_val->type == AST_CHARACTER)
  {
    if (left_val->string_value != NULL && right_val->char_value.null == 0)
    {
      result = (char *)malloc(strlen(left_val->string_value) + 2); // 1 char + null terminator
      if (result == NULL)
      {
        perror("Failed to allocate memory for string-char concatenation");
        exit(1);
      }
      strcpy(result, left_val->string_value);
      result[strlen(left_val->string_value)] = right_val->char_value.value; // Access the char value
      result[strlen(left_val->string_value) + 1] = '\0';
    }
    else
    {
      perror("Concatenation error: One of the values is null");
      exit(1);
    }
  }
  // Handle character + string
  else if (left_val->type == AST_CHARACTER && right_val->type == AST_STRING)
  {
    if (left_val->char_value.null == 0 && right_val->string_value != NULL)
    {
      result = (char *)malloc(2 + strlen(right_val->string_value)); // 1 char + string + null terminator
      if (result == NULL)
      {
        perror("Failed to allocate memory for char-string concatenation");
        exit(1);
      }
      result[0] = left_val->char_value.value; // Access the char value
      strcpy(&result[1], right_val->string_value);
    }
    else
    {
      perror("Concatenation error: One of the values is null");
      exit(1);
    }
  }
  // Handle string + string
  else if (left_val->type == AST_STRING && right_val->type == AST_STRING)
  {
    if (left_val->string_value != NULL && right_val->string_value != NULL)
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
    else
    {
      perror("Concatenation error: One of the string values is null");
      exit(1);
    }
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
    exit(1);
  }

  if (array->type != AST_ARRAY)
  {
    fprintf(stderr, "Error: %s is not defined as an array.\n", node->variable_name);
    exit(1);
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
      exit(1);
    }
    if (index_value->int_value.null == 0)
    {
      int index = index_value->int_value.value;

      // Ensure that current_array is an array and the index is within bounds
      if (current_array->type != AST_ARRAY || index < 0 || index >= current_array->array_size)
      {
        printf("Error: Array index out of bounds or invalid array access.\n");
        exit(1);
      }

      // Move to the next nested array or value
      current_array = current_array->array_elements[index];
      b++;
    }
    else
    {
      printf("Error: Array index cannot be null.\n");
      exit(1);
    }
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
  ast_ *left_val = NULL;
  ast_ *right_val = NULL;

  // Process the left side, if not null
  if (node->left != NULL)
  {
    left_val = interpreter_process(interpreter, node->left);
  }

  // Process the right side, which should always exist
  right_val = interpreter_process(interpreter, node->right);

  // Check for null values on both sides before performing operations
  if (left_val != NULL && left_val->int_value.null == 1)
  {
    perror("Left operand is null");
    exit(1);
  }

  if (right_val != NULL && right_val->int_value.null == 1)
  {
    perror("Right operand is null");
    exit(1);
  }

  // Handle unary minus operation
  if ((strcmp(node->op, "-") == 0) && node->left == NULL)
  {
    if (right_val->type == AST_INTEGER && right_val->int_value.null == 0)
    {
      right_val->int_value.value = -(right_val->int_value.value);
      return right_val;
    }
    else if (right_val->type == AST_REAL && right_val->real_value.null == 0)
    {
      right_val->real_value.value = -(right_val->real_value.value);
      return right_val;
    }
  }

  // Handle binary operations
  if (strcmp(node->op, "+") == 0)
  {
    if ((left_val->type == AST_CHARACTER && left_val->char_value.null == 0) ||
        (right_val->type == AST_CHARACTER && right_val->char_value.null == 0))
    {
      return concatenate(left_val, right_val); // General concatenation for char/string
    }

    else if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER &&
             left_val->int_value.null == 0 && right_val->int_value.null == 0)
    {
      left_val->int_value.value += right_val->int_value.value;
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL &&
             left_val->real_value.null == 0 && right_val->real_value.null == 0)
    {
      left_val->real_value.value += right_val->real_value.value;
      return left_val;
    }
  }
  else if (strcmp(node->op, "-") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER &&
        left_val->int_value.null == 0 && right_val->int_value.null == 0)
    {
      left_val->int_value.value -= right_val->int_value.value;
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL &&
             left_val->real_value.null == 0 && right_val->real_value.null == 0)
    {
      left_val->real_value.value -= right_val->real_value.value;
      return left_val;
    }
  }
  else if (strcmp(node->op, "*") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER &&
        left_val->int_value.null == 0 && right_val->int_value.null == 0)
    {
      left_val->int_value.value *= right_val->int_value.value;
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL &&
             left_val->real_value.null == 0 && right_val->real_value.null == 0)
    {
      left_val->real_value.value *= right_val->real_value.value;
      return left_val;
    }
  }
  else if (strcmp(node->op, "/") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER &&
        left_val->int_value.null == 0 && right_val->int_value.null == 0)
    {
      left_val->int_value.value /= right_val->int_value.value;
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL &&
             left_val->real_value.null == 0 && right_val->real_value.null == 0)
    {
      left_val->real_value.value /= right_val->real_value.value;
      return left_val;
    }
  }
  else if (strcmp(node->op, "^") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER &&
        left_val->int_value.null == 0 && right_val->int_value.null == 0)
    {
      left_val->int_value.value = pow(left_val->int_value.value, right_val->int_value.value);
      return left_val;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL &&
             left_val->real_value.null == 0 && right_val->real_value.null == 0)
    {
      left_val->real_value.value = pow(left_val->real_value.value, right_val->real_value.value);
      return left_val;
    }
  }
  else if (strcmp(node->op, "DIV") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER &&
        left_val->int_value.null == 0 && right_val->int_value.null == 0)
    {
      left_val->int_value.value /= right_val->int_value.value; // Integer division
      return left_val;
    }
  }
  else if (strcmp(node->op, "MOD") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER &&
        left_val->int_value.null == 0 && right_val->int_value.null == 0)
    {
      left_val->int_value.value %= right_val->int_value.value;
      return left_val;
    }
  }

  printf("Invalid arithmetic operation: %s\n", node->op);
  exit(1);
  return init_ast(AST_NOOP); // Return a no-op if no valid operation found
}

ast_ *interpreter_process_boolean_expression(interpreter_ *interpreter, ast_ *node)
{
  // Evaluate left and right sides if applicable (AND/OR need both sides, NOT needs only one)
  ast_ *left_val = NULL;
  ast_ *right_val = NULL;

  if (strcmp(node->op, "NOT") != 0)
  {
    left_val = interpreter_process(interpreter, node->left);
    right_val = interpreter_process(interpreter, node->right);

    // Ensure both values are not null before proceeding
    if ((left_val->type == AST_INTEGER && left_val->int_value.null == 1) ||
        (right_val->type == AST_INTEGER && right_val->int_value.null == 1) ||
        (left_val->type == AST_REAL && left_val->real_value.null == 1) ||
        (right_val->type == AST_REAL && right_val->real_value.null == 1) ||
        (left_val->type == AST_CHARACTER && left_val->char_value.null == 1) ||
        (right_val->type == AST_CHARACTER && right_val->char_value.null == 1) ||
        (left_val->type == AST_STRING && left_val->string_value == NULL) ||
        (right_val->type == AST_STRING && right_val->string_value == NULL))
    {
      printf("Error: Null value in boolean expression\n");
      exit(1);
    }

    // Ensure both sides are of comparable types (int, real, char, string)
    if (left_val->type != right_val->type)
    {
      printf("Error: Type mismatch in boolean expression\n");
      exit(1);
    }
  }
  else
  {
    // Evaluate only the right side for 'NOT' operation
    right_val = interpreter_process(interpreter, node->right);
    if ((right_val->type == AST_INTEGER && right_val->int_value.null == 1) ||
        (right_val->type == AST_REAL && right_val->real_value.null == 1) ||
        (right_val->type == AST_CHARACTER && right_val->char_value.null == 1) ||
        (right_val->type == AST_STRING && right_val->string_value == NULL))
    {
      printf("Error: Null value in boolean expression\n");
      exit(1);
    }
  }

  ast_ *result = init_ast(AST_BOOLEAN);

  // Handle logical operations 'AND' and 'OR'
  if (strcmp(node->op, "AND") == 0)
  {
    result->boolean_value.value = (left_val->boolean_value.value && right_val->boolean_value.value);
  }
  else if (strcmp(node->op, "OR") == 0)
  {
    result->boolean_value.value = (left_val->boolean_value.value || right_val->boolean_value.value);
  }
  else if (strcmp(node->op, "NOT") == 0)
  {
    result->boolean_value.value = !right_val->boolean_value.value;
  }
  // Handle comparisons for int and real
  else if (left_val->type == AST_INTEGER || left_val->type == AST_REAL)
  {
    double left_num = (left_val->type == AST_INTEGER) ? left_val->int_value.value : left_val->real_value.value;
    double right_num = (right_val->type == AST_INTEGER) ? right_val->int_value.value : right_val->real_value.value;

    if (strcmp(node->op, "<") == 0)
      result->boolean_value.value = left_num < right_num;
    else if (strcmp(node->op, ">") == 0)
      result->boolean_value.value = left_num > right_num;
    else if (strcmp(node->op, "<=") == 0)
      result->boolean_value.value = left_num <= right_num;
    else if (strcmp(node->op, ">=") == 0)
      result->boolean_value.value = left_num >= right_num;
    else if (strcmp(node->op, "=") == 0)
      result->boolean_value.value = left_num == right_num;
    else if (strcmp(node->op, "!=") == 0)
      result->boolean_value.value = left_num != right_num;
    else
    {
      printf("Invalid boolean operation: %s\n", node->op);
      exit(1);
    }
  }
  // Handle comparisons for char (based on ASCII values)
  else if (left_val->type == AST_CHARACTER)
  {
    char left_char = left_val->char_value.value;
    char right_char = right_val->char_value.value;

    if (strcmp(node->op, "<") == 0)
      result->boolean_value.value = left_char < right_char;
    else if (strcmp(node->op, ">") == 0)
      result->boolean_value.value = left_char > right_char;
    else if (strcmp(node->op, "<=") == 0)
      result->boolean_value.value = left_char <= right_char;
    else if (strcmp(node->op, ">=") == 0)
      result->boolean_value.value = left_char >= right_char;
    else if (strcmp(node->op, "=") == 0)
      result->boolean_value.value = left_char == right_char;
    else if (strcmp(node->op, "!=") == 0)
      result->boolean_value.value = left_char != right_char;
    else
    {
      printf("Invalid boolean operation: %s\n", node->op);
      exit(1);
    }
  }
  // Handle comparisons for strings (lexicographical comparison)
  else if (left_val->type == AST_STRING)
  {
    int cmp_result = strcmp(left_val->string_value, right_val->string_value);

    if (strcmp(node->op, "<") == 0)
      result->boolean_value.value = cmp_result < 0;
    else if (strcmp(node->op, ">") == 0)
      result->boolean_value.value = cmp_result > 0;
    else if (strcmp(node->op, "<=") == 0)
      result->boolean_value.value = cmp_result <= 0;
    else if (strcmp(node->op, ">=") == 0)
      result->boolean_value.value = cmp_result >= 0;
    else if (strcmp(node->op, "=") == 0)
      result->boolean_value.value = cmp_result == 0;
    else if (strcmp(node->op, "!=") == 0)
      result->boolean_value.value = cmp_result != 0;
    else
    {
      printf("Invalid boolean operation: %s\n", node->op);
      exit(1);
    }
  }
  else
  {
    printf("Error: Unsupported type for boolean comparison\n");
    exit(1);
  }

  result->boolean_value.null = 0;
  return result;
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
      if (expr->string_value != NULL)
      {
        printf("%s", expr->string_value);
      }
      break;

    case AST_INTEGER:
      if (expr->int_value.null == 0)
      {
        printf("%d", expr->int_value.value);
      }
      break;

    case AST_REAL:
      if (expr->real_value.null == 0)
      {
        printf("%0.2f", expr->real_value.value);
      }
      break;

    case AST_CHARACTER:
      if (expr->char_value.null == 0)
      {
        printf("%c", expr->char_value.value);
      }
      break;

    case AST_BOOLEAN:
      if (expr->boolean_value.null == 0)
      {
        printf("%s", expr->boolean_value.value ? "True" : "False");
      }
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
