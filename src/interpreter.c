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

void interpreter_output_literal(ast_ *expr, interpreter_ *interpreter)
{

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

  case AST_ARRAY:
    if (expr->array_elements != NULL && expr->array_size != 0)
    {
      printf("[");
      for (size_t j = 0; j < expr->array_size; j++)
      {
        interpreter_output_literal(expr->array_elements[j], interpreter);
        if (j < expr->array_size - 1)
        {
          printf(", ");
        }
      }
      printf("]");
    }
    break;

  case AST_RECORD:
    if (expr->record_name != NULL && expr->record_elements != NULL)
    {
      printf("%s {\n", expr->record_name);
      for (size_t k = 0; k < expr->field_count; k++)
      {
        printf("  %s: ", expr->record_elements[k]->element_name);
        interpreter_output_literal(expr->record_elements[k]->element, interpreter);
        printf("\n");
      }
      printf("}");
    }
    break;

  default:
    fprintf(stderr, "Unsupported output type in AST_OUTPUT.\n");
    exit(1);
  }
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
  case AST_INTEGER:
    return node;
  case AST_REAL:
    return node;
  case AST_CHARACTER:
    return node;
  case AST_STRING:
    return node;
  case AST_BOOLEAN:
    return node;
  case AST_ARRAY:
    return node;
  case AST_RECORD:
    return node;
  case AST_ASSIGNMENT:
    return interpreter_process_assignment(interpreter, node);
  case AST_VARIABLE:
    return interpreter_process_variable(interpreter, node);
  case AST_RECORD_DEFINITION_ACCESS:
    return interpreter_process_record_access(interpreter, node);
  case AST_ARRAY_ACCESS:
    return interpreter_process_array_access(interpreter, node);
  case AST_INSTANTIATION:
    return interpreter_process_instantiation(interpreter, node);
  case AST_ARITHMETIC_EXPRESSION:
    return interpreter_process_arithmetic_expression(interpreter, node);
  case AST_BOOLEAN_EXPRESSION:
    return interpreter_process_boolean_expression(interpreter, node);
  case AST_RECORD_DEFINITION:
    return interpreter_process_record_definition(interpreter, node);
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
  int i = 0;
  while (node->compound_value[i] != NULL)
  {
    interpreter_process(interpreter, node->compound_value[i]);
    i++;
  }
  return init_ast(AST_NOOP);
}

ast_ *interpreter_process_assignment(interpreter_ *interpreter, ast_ *node)
{
  // Process right-hand side before storing in scope
  ast_ *rhs_value = interpreter_process(interpreter, node->rhs);
  node->rhs = rhs_value; // Update the right-hand side with the processed value

  // Add the variable to the scope
  scope_add_variable_definition(node->scope, node);

  return node;
}
ast_ *interpreter_process_variable(interpreter_ *interpreter, ast_ *node)
{
  ast_ *vdef = scope_get_variable_definition(node->scope, node->variable_name);

  if (vdef != NULL)
  {
    return interpreter_process(interpreter, vdef);
  }

  printf("Undefined variable `%s`\n", node->variable_name);
  exit(1);
  return init_ast(AST_NOOP);
}
ast_ *interpreter_process_record_access(interpreter_ *interpreter, ast_ *node)
{
  printf(">> record_access <<\n");
  return init_ast(AST_NOOP);
}
ast_ *interpreter_process_array_access(interpreter_ *interpreter, ast_ *node)
{
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

  // Use scope_get_instantiation_definition to find the record definition
  ast_ *inst_definition = scope_get_instantiation_definition(node->scope, node->class_name);

  if (!inst_definition)
  {
    printf("Error: Record definition '%s' not found.\n", node->class_name);
    return init_ast(AST_NOOP);
  }

  // Check if the number of arguments matches the number of fields in the record
  if (node->arguments_count != inst_definition->field_count)
  {
    printf("Error: Mismatched number of arguments for record instantiation '%s'.\n", node->class_name);
    return init_ast(AST_NOOP);
  }

  // Create a new AST_RECORD_DEFINITION node for the instantiated record
  ast_ *new_record = init_ast(AST_RECORD_DEFINITION);
  new_record->record_name = node->class_name;
  new_record->record_elements = (ast_record_element_ **)init_ast_list();
  new_record->field_count = 0;

  printf("New instantiated record\n");

  // Assign the values from the arguments to the corresponding fields
  for (int i = 0; i < node->arguments_count; i++)
  {
    // Match the type of the argument with the record field type
    if (node->arguments[i]->type == inst_definition->record_elements[i]->element->type)
    {
      printf("arg type : %s\n", ast_type_to_string(node->arguments[i]->type));
      printf("record type : %s\n", ast_type_to_string(inst_definition->record_elements[i]->element->type));

      // Allocate memory for the record element
      ast_record_element_ *record_element = malloc(sizeof(ast_record_element_));
      if (!record_element)
      {
        printf("Memory allocation error.\n");
        exit(1);
      }
      // Set the element name from the instantiation definition
      record_element->element_name = inst_definition->record_elements[i]->element_name;

      // Initialize the value element based on the argument's type
      ast_ *element = init_ast(node->arguments[i]->type);
      switch (element->type)
      {
      case AST_INTEGER:
        element->int_value = node->arguments[i]->int_value;
        printf("Assigning integer %d to field %s\n", element->int_value.value, record_element->element_name);
        break;
      case AST_REAL:
        element->real_value = node->arguments[i]->real_value;
        printf("Assigning real %.2f to field %s\n", element->real_value.value, record_element->element_name);
        break;
      case AST_CHARACTER:
        element->char_value = node->arguments[i]->char_value;
        printf("Assigning character %c to field %s\n", element->char_value.value, record_element->element_name);
        break;
      case AST_STRING:
        element->string_value = node->arguments[i]->string_value;
        printf("Assigning string %s to field %s\n", element->string_value, record_element->element_name);
        break;
      case AST_BOOLEAN:
        element->boolean_value = node->arguments[i]->boolean_value;
        printf("Assigning boolean %d to field %s\n", element->boolean_value.value, record_element->element_name);
        break;
      case AST_ARRAY:
        printf("Assigning array to field %s\n", record_element->element_name);
        element->array_elements = node->arguments[i]->array_elements;
        element->array_size = node->arguments[i]->array_size;
        break;
      default:
        fprintf(stderr, "Error: Unsupported type for record instantiation.\n");
        exit(1);
      }

      // Assign the element to the record element
      record_element->element = element;
      record_element->dimension = 0;

      // Add the record element to the list of record elements
      add_ast_to_list((ast_ ***)&new_record->record_elements, (ast_ *)record_element);
      new_record->field_count++;

      printf("Added to record {%s, %s}\n", record_element->element_name, ast_type_to_string(record_element->element->type));
    }
    else
    {
      printf("Error: Mismatched type for argument %d in record instantiation '%s'.\n", i, node->class_name);
      return init_ast(AST_NOOP);
    }
  }

  return new_record;
}

ast_ *interpreter_process_arithmetic_expression(interpreter_ *interpreter, ast_ *node)
{
  ast_ *left_val = NULL;
  ast_ *right_val = NULL;

  if (node->left != NULL)
  {
    left_val = interpreter_process(interpreter, node->left);
  }

  right_val = interpreter_process(interpreter, node->right);

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
  return init_ast(AST_NOOP);
}
ast_ *interpreter_process_boolean_expression(interpreter_ *interpreter, ast_ *node)
{
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

ast_ *interpreter_process_record_definition(interpreter_ *interpreter, ast_ *node)
{
  printf(">> record definition <<\n");
  scope_add_instantiation_definition(
      node->scope,
      node);

  return node;
}
ast_ *interpreter_process_subroutine(interpreter_ *interpreter, ast_ *node)
{
  printf(">> subroutine <<\n");
  scope_add_instantiation_definition(
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
    interpreter_output_literal(expr, interpreter);

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
