#include "include/interpreter.h"
#include "include/scope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

int compare_ast_literals(ast_ *a, ast_ *b)
{
  // First, check if the types match
  if (a->type != b->type)
  {
    fprintf(stderr, "Mismatched types during comparison\n");
    return 0; // Types don't match, return 0 (false)
  }

  // Switch based on the AST type and compare values
  switch (b->type)
  {
  case AST_STRING:
    return (a->string_value != NULL && b->string_value != NULL &&
            strcmp(a->string_value, b->string_value) == 0);

  case AST_INTEGER:
    return (a->int_value.null == 0 && b->int_value.null == 0 &&
            a->int_value.value == b->int_value.value);

  case AST_REAL:
    return (a->real_value.null == 0 && b->real_value.null == 0 &&
            a->real_value.value == b->real_value.value);

  case AST_CHARACTER:
    return (a->char_value.null == 0 && b->char_value.null == 0 &&
            a->char_value.value == b->char_value.value);

  case AST_BOOLEAN:
    return (a->boolean_value.null == 0 && b->boolean_value.null == 0 &&
            a->boolean_value.value == b->boolean_value.value);

  case AST_ARRAY:
    if (a->array_size != b->array_size)
    {
      return 0; // Arrays differ in size
    }

    // For each element in the array, compare individually
    for (size_t i = 0; i < a->array_size; i++)
    {
      // Compare the elements of the array using compare_ast_literals (recursion for nested arrays)
      if (!compare_ast_literals(a->array_elements[i], b->array_elements[i]))
      {
        return 0; // Element mismatch found
      }
    }
    return 1; // Arrays match in size and elements

  default:
    fprintf(stderr, "Unsupported AST type in compare_ast_literals.\n");
    exit(1);
  }

  return 0; // Fallback, in case no match was found
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
  case AST_RECORD_DEFINITION:
    return interpreter_process_record_definition(interpreter, node);
  case AST_SUBROUTINE:
    return interpreter_process_subroutine(interpreter, node);
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

  // Create a new node for assignment, keeping the original node intact
  ast_ *new_assignment = init_ast(AST_ASSIGNMENT);
  new_assignment->lhs = node->lhs; // Keep the left-hand side (variable) the same
  new_assignment->rhs = rhs_value; // Use the processed right-hand side value

  // Add the variable and its value to the scope
  scope_add_variable_definition(get_scope(node), new_assignment);

  return new_assignment; // Return the new assignment node
}

ast_ *interpreter_process_variable(interpreter_ *interpreter, ast_ *node)
{
  ast_ *vdef = scope_get_variable_definition(get_scope(node), node->variable_name);

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
  ast_ *array = scope_get_variable_definition(get_scope(node), node->variable_name);

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
  // Processing built-in methods
  if (strcmp(node->class_name, "LEN") == 0)
  {

    // Check if the argument count is exactly 1
    if (node->arguments_count != 1)
    {
      fprintf(stderr, "Error: LEN method expects 1 argument, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the argument (assuming it's stored in node->arguments[0])
    ast_ *arg = interpreter_process(interpreter, node->arguments[0]);

    // Check if the argument is an array or string
    if (arg->type == AST_ARRAY)
    {
      // Return the size of the array
      ast_ *return_value = init_ast(AST_INTEGER);
      return_value->int_value.value = arg->array_size;
      return_value->int_value.null = 0;
      return return_value;
    }
    else if (arg->type == AST_STRING)
    {
      // Return the length of the string
      ast_ *return_value = init_ast(AST_INTEGER);
      return_value->int_value.value = strlen(arg->string_value);
      return_value->int_value.null = 0;
      return return_value;
    }
    else
    {
      // Invalid argument type
      fprintf(stderr, "Error: LEN method expects an array or a string.\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "POSITION") == 0)
  {
    // Check if the argument count is exactly 2
    if (node->arguments_count != 2)
    {
      fprintf(stderr, "Error: POS method expects 2 arguments, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the arguments
    ast_ *arg0 = interpreter_process(interpreter, node->arguments[0]); // The array or string
    ast_ *arg1 = interpreter_process(interpreter, node->arguments[1]); // The value to find

    // Check if arg0 is an array and arg1 has the same type as the first element of the array
    if (arg0->type == AST_ARRAY && arg1->type == arg0->array_elements[0]->type)
    {
      for (int i = 0; i < arg0->array_size; i++)
      {
        // Assuming you have a function to compare array elements
        if (compare_ast_literals(arg0->array_elements[i], arg1) == 1)
        {
          ast_ *return_value = init_ast(AST_INTEGER);
          return_value->int_value.value = i;
          return_value->int_value.null = 0;
          return return_value;
        }
      }
      // If element is not found, return an error or a special value (-1)
      fprintf(stderr, "Error: Element not found in array.\n");

      return init_ast(AST_NOOP);
    }
    // Check if arg0 is a string and arg1 is a character
    else if (arg0->type == AST_STRING && arg1->type == AST_CHARACTER)
    {
      for (int i = 0; i < strlen(arg0->string_value); i++)
      {
        if (arg0->string_value[i] == arg1->char_value.value)
        {
          ast_ *return_value = init_ast(AST_INTEGER);
          return_value->int_value.value = i;
          return_value->int_value.null = 0;
          return return_value;
        }
      }
      // If character is not found, return an error or a special value (-1)
      fprintf(stderr, "Error: Character not found in string.\n");
      return init_ast(AST_NOOP);
    }
    else
    {
      // Invalid argument types
      fprintf(stderr, "Error: POS method expects an array or a string.\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "SUBSTRING") == 0)
  {
    // Check if the argument count is exactly 3
    if (node->arguments_count != 3)
    {
      fprintf(stderr, "Error: SUBSTRING method expects 3 arguments, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the arguments
    ast_ *string = interpreter_process(interpreter, node->arguments[0]);      // The string
    ast_ *start_value = interpreter_process(interpreter, node->arguments[1]); // Start index
    ast_ *end_value = interpreter_process(interpreter, node->arguments[2]);   // End index

    // Validate argument types: the first argument must be a string, the others must be integers
    if (string->type == AST_STRING && start_value->type == AST_INTEGER && end_value->type == AST_INTEGER)
    {
      // Get the string, start, and end values
      char *input_string = string->string_value;
      int start = start_value->int_value.value;
      int end = end_value->int_value.value + 1;
      int string_length = strlen(input_string);

      // Validate the start and end values to ensure they are within the string bounds
      if (start < 0 || start >= string_length || end < 0 || end > string_length || start > end)
      {
        fprintf(stderr, "Error: SUBSTRING indices out of bounds or invalid. Start: %d, End: %d, String Length: %d\n", start, end, string_length);
        return init_ast(AST_NOOP);
      }

      // Calculate the length of the substring
      int substring_length = end - start;

      // Allocate memory for the new substring
      char *substring = (char *)malloc(substring_length + 1);
      if (substring == NULL)
      {
        fprintf(stderr, "Error: Memory allocation for SUBSTRING failed.\n");
        exit(1);
      }

      // Copy the substring from the input string
      strncpy(substring, input_string + start, substring_length);
      substring[substring_length] = '\0'; // Null-terminate the substring

      // Create an AST node for the substring result
      ast_ *return_value = init_ast(AST_STRING);
      return_value->string_value = substring;

      return return_value;
    }
    else
    {
      // Invalid argument types
      fprintf(stderr, "Error: SUBSTRING method expects a string followed by 2 integers.\n SUBSTRING(string, integer, integer)\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "SLICE") == 0)
  {
    // Check if the argument count is exactly 3
    if (node->arguments_count != 3)
    {
      fprintf(stderr, "Error: SLICE method expects 3 arguments, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the arguments
    ast_ *array = interpreter_process(interpreter, node->arguments[0]);       // The array or matrix
    ast_ *start_value = interpreter_process(interpreter, node->arguments[1]); // Start index
    ast_ *end_value = interpreter_process(interpreter, node->arguments[2]);   // End index

    // Validate argument types: the first argument must be an array, the others must be integers
    if (array->type == AST_ARRAY && start_value->type == AST_INTEGER && end_value->type == AST_INTEGER)
    {
      // Get the start, end, and array details
      int start = start_value->int_value.value;
      int end = end_value->int_value.value + 1;
      int array_size = array->array_size;

      // Validate the start and end values to ensure they are within the array bounds
      if (start < 0 || start >= array_size || end < 0 || end > array_size || start > end)
      {
        fprintf(stderr, "Error: SLICE indices out of bounds or invalid. Start: %d, End: %d, Array Size: %d\n", start, end, array_size);
        return init_ast(AST_NOOP);
      }

      // Initialize the new array slice
      ast_ *slice = init_ast(AST_ARRAY);
      slice->array_elements = init_ast_list(); // Initialize the list of array elements

      // Iterate through the original array and add the slice elements
      for (int i = start; i < end; i++)
      {
        ast_ *element = array->array_elements[i];
        add_ast_to_list(&(slice->array_elements), element); // Add the element to the slice
      }

      // Set the size of the slice
      slice->array_size = end - start;

      return slice;
    }
    else
    {
      // Invalid argument types
      fprintf(stderr, "Error: SLICE method expects an array followed by 2 integers.\n SLICE(array, integer, integer)\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "STRING_TO_INT") == 0)
  {
    // Check if the argument count is exactly 1
    if (node->arguments_count != 1)
    {
      fprintf(stderr, "Error: STRING_TO_INT method expects 1 argument, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the argument and check if it’s a string
    ast_ *string_arg = interpreter_process(interpreter, node->arguments[0]);
    if (string_arg->type == AST_STRING)
    {
      int int_value = atoi(string_arg->string_value); // Convert string to integer
      ast_ *return_value = init_ast(AST_INTEGER);
      return_value->int_value.value = int_value;
      return_value->int_value.null = 0;
      return return_value;
    }
    else
    {
      fprintf(stderr, "Error: STRING_TO_INT method expects a string argument.\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "STRING_TO_REAL") == 0)
  {
    // Check if the argument count is exactly 1
    if (node->arguments_count != 1)
    {
      fprintf(stderr, "Error: STRING_TO_REAL method expects 1 argument, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the argument and check if it’s a string
    ast_ *string_arg = interpreter_process(interpreter, node->arguments[0]);
    if (string_arg->type == AST_STRING)
    {
      double real_value = atof(string_arg->string_value); // Convert string to real (double)
      ast_ *return_value = init_ast(AST_REAL);
      return_value->real_value.value = real_value;
      return_value->real_value.null = 0;
      return return_value;
    }
    else
    {
      fprintf(stderr, "Error: STRING_TO_REAL method expects a string argument.\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "INT_TO_STRING") == 0)
  {
    // Check if the argument count is exactly 1
    if (node->arguments_count != 1)
    {
      fprintf(stderr, "Error: INT_TO_STRING method expects 1 argument, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the argument and check if it’s an integer
    ast_ *int_arg = interpreter_process(interpreter, node->arguments[0]);
    if (int_arg->type == AST_INTEGER)
    {
      char buffer[12];                                                  // Buffer large enough to hold any 32-bit integer
      snprintf(buffer, sizeof(buffer), "%d", int_arg->int_value.value); // Convert integer to string
      ast_ *return_value = init_ast(AST_STRING);
      return_value->string_value = strdup(buffer); // Copy the result string
      return return_value;
    }
    else
    {
      fprintf(stderr, "Error: INT_TO_STRING method expects an integer argument.\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "REAL_TO_STRING") == 0)
  {
    // Check if the argument count is exactly 1
    if (node->arguments_count != 1)
    {
      fprintf(stderr, "Error: REAL_TO_STRING method expects 1 argument, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the argument and check if it’s a real (float/double)
    ast_ *real_arg = interpreter_process(interpreter, node->arguments[0]);
    if (real_arg->type == AST_REAL)
    {
      char buffer[32];                                                    // Buffer large enough to hold any double
      snprintf(buffer, sizeof(buffer), "%f", real_arg->real_value.value); // Convert real to string
      ast_ *return_value = init_ast(AST_STRING);
      return_value->string_value = strdup(buffer); // Copy the result string
      return return_value;
    }
    else
    {
      fprintf(stderr, "Error: REAL_TO_STRING method expects a real argument.\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "CHAR_TO_CODE") == 0)
  {
    // Check if the argument count is exactly 1
    if (node->arguments_count != 1)
    {
      fprintf(stderr, "Error: CHAR_TO_CODE method expects 1 argument, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the argument and check if it’s a character
    ast_ *char_arg = interpreter_process(interpreter, node->arguments[0]);
    if (char_arg->type == AST_CHARACTER)
    {
      int char_code = (int)char_arg->char_value.value; // Convert char to ASCII code
      ast_ *return_value = init_ast(AST_INTEGER);
      return_value->int_value.value = char_code;
      return_value->int_value.null = 0;
      return return_value;
    }
    else
    {
      fprintf(stderr, "Error: CHAR_TO_CODE method expects a character argument.\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "CODE_TO_CHAR") == 0)
  {
    // Check if the argument count is exactly 1
    if (node->arguments_count != 1)
    {
      fprintf(stderr, "Error: CODE_TO_CHAR method expects 1 argument, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the argument and check if it’s an integer (ASCII code)
    ast_ *int_arg = interpreter_process(interpreter, node->arguments[0]);
    if (int_arg->type == AST_INTEGER)
    {
      char char_value = (char)int_arg->int_value.value; // Convert ASCII code to char
      ast_ *return_value = init_ast(AST_CHARACTER);
      return_value->char_value.value = char_value;
      return_value->char_value.null = 0;
      return return_value;
    }
    else
    {
      fprintf(stderr, "Error: CODE_TO_CHAR method expects an integer argument.\n");
      return init_ast(AST_NOOP);
    }
  }
  else if (strcmp(node->class_name, "RANDOM_INT") == 0)
  {
    // Check if the argument count is exactly 2
    if (node->arguments_count != 2)
    {
      fprintf(stderr, "Error: RANDOM_INT method expects 2 arguments, but got %d.\n", node->arguments_count);
      return init_ast(AST_NOOP);
    }

    // Retrieve the arguments and check if they are integers
    ast_ *min_value = interpreter_process(interpreter, node->arguments[0]);
    ast_ *max_value = interpreter_process(interpreter, node->arguments[1]);

    if (min_value->type == AST_INTEGER && max_value->type == AST_INTEGER)
    {
      int min = min_value->int_value.value;
      int max = max_value->int_value.value;

      // Seed the random number generator if necessary (e.g., in program initialization)
      srand(time(NULL)); // Ensure this is called only once in your program's lifecycle

      // Generate a random integer between min and max (inclusive)
      int random_value = min + rand() % (max - min + 1);

      ast_ *return_value = init_ast(AST_INTEGER);
      return_value->int_value.value = random_value;
      return_value->int_value.null = 0;
      return return_value;
    }
    else
    {
      fprintf(stderr, "Error: RANDOM_INT method expects two integer arguments.\n");
      return init_ast(AST_NOOP);
    }
  }

  // Use scope_get_instantiation_definition to find the record definition
  ast_ *inst_definition = scope_get_instantiation_definition(get_scope(node), node->class_name);

  if (!inst_definition)
  {
    if (node->type == AST_RECORD_DEFINITION)
    {
      printf("Error: Record definition '%s' not found.\n", node->class_name);
    }
    else if (node->type == AST_SUBROUTINE)
    {
      printf("Error: Subroutine definition '%s' not found.\n", node->class_name);
    }
    exit(1);
  }

  if (inst_definition->type == AST_RECORD_DEFINITION && (node->arguments_count != inst_definition->field_count))
  {
    printf("Error: Mismatched number of arguments for record instantiation '%s'.\n", node->class_name);
    exit(1);
  }
  else if (inst_definition->type == AST_SUBROUTINE && (node->arguments_count != inst_definition->parameter_count))
  {
    printf("Error: Mismatched number of arguments for subroutine instantiation '%s'.\n", node->class_name);
    exit(1);
  }

  if (inst_definition->type == AST_RECORD_DEFINITION)
  {
    // Create a new AST_RECORD node for the instantiated record
    ast_ *new_record = init_ast(AST_RECORD);
    new_record->record_name = node->class_name;
    new_record->record_elements = (ast_record_element_ **)init_ast_list();
    new_record->field_count = 0;

    // Assign the values from the arguments to the corresponding fields
    for (int i = 0; i < node->arguments_count; i++)
    {
      // Match the type of the argument with the record field type
      if (node->arguments[i]->type == inst_definition->record_elements[i]->element->type)
      {
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
          break;
        case AST_REAL:
          element->real_value = node->arguments[i]->real_value;
          break;
        case AST_CHARACTER:
          element->char_value = node->arguments[i]->char_value;
          break;
        case AST_STRING:
          element->string_value = node->arguments[i]->string_value;
          break;
        case AST_BOOLEAN:
          element->boolean_value = node->arguments[i]->boolean_value;
          break;
        case AST_ARRAY:
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
      }
      else
      {
        printf("Error: Mismatched type for argument %d in record instantiation '%s'.\n", i, node->class_name);
        return init_ast(AST_NOOP);
      }
    }

    return new_record;
  }
  else if (inst_definition->type == AST_SUBROUTINE)
  {
    ast_ *inst_definition_copy = deep_copy(inst_definition);
    // Assign the arguments to the subroutine's parameters within this new scope
    for (int i = 0; i < node->arguments_count; i++)
    {
      ast_ *var = init_ast(AST_ASSIGNMENT);
      var->lhs = init_ast(AST_VARIABLE);
      var->lhs->variable_name = strdup(inst_definition_copy->parameters[i]->variable_name);
      var->rhs = init_ast(node->arguments[i]->type);
      *var->rhs = *node->arguments[i];

      // Add the variable to the subroutine-specific scope
      scope_add_variable_definition(node->scope, var);
    }

    // Process the function body within this new scope
    for (int i = 0; inst_definition_copy->body[i] != NULL; i++)
    {
      ast_ *current_statement = inst_definition_copy->body[i];
      set_scope(current_statement, node->scope);

      if (current_statement->type == AST_RETURN)
      {
        return interpreter_process(interpreter, current_statement->return_value);
      }

      interpreter_process(interpreter, current_statement);
    }
  }

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

  // Check for null values before proceeding
  if ((left_val && ((left_val->type == AST_INTEGER && left_val->int_value.null == 1) ||
                    (left_val->type == AST_REAL && left_val->real_value.null == 1) ||
                    (left_val->type == AST_CHARACTER && left_val->char_value.null == 1) ||
                    (left_val->type == AST_STRING && left_val->string_value == NULL))) ||
      (right_val && ((right_val->type == AST_INTEGER && right_val->int_value.null == 1) ||
                     (right_val->type == AST_REAL && right_val->real_value.null == 1) ||
                     (right_val->type == AST_CHARACTER && right_val->char_value.null == 1) ||
                     (right_val->type == AST_STRING && right_val->string_value == NULL))))
  {
    printf("Error: Null value in arithmetic expression\n");
    exit(1);
  }

  // Handle unary minus operation
  if ((strcmp(node->op, "-") == 0) && node->left == NULL)
  {
    if (right_val->type == AST_INTEGER)
    {
      ast_ *result = init_ast(AST_INTEGER);
      result->int_value.value = -(right_val->int_value.value);
      result->int_value.null = 0;
      return result;
    }
    else if (right_val->type == AST_REAL)
    {
      ast_ *result = init_ast(AST_REAL);
      result->real_value.value = -(right_val->real_value.value);
      result->real_value.null = 0;
      return result;
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
      ast_ *result = init_ast(AST_INTEGER);
      result->int_value.value = left_val->int_value.value + right_val->int_value.value;
      result->int_value.null = 0;
      return result;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      ast_ *result = init_ast(AST_REAL);
      result->real_value.value = left_val->real_value.value + right_val->real_value.value;
      result->real_value.null = 0;
      return result;
    }
  }
  else if (strcmp(node->op, "-") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      ast_ *result = init_ast(AST_INTEGER);
      result->int_value.value = left_val->int_value.value - right_val->int_value.value;
      result->int_value.null = 0;
      return result;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      ast_ *result = init_ast(AST_REAL);
      result->real_value.value = left_val->real_value.value - right_val->real_value.value;
      result->real_value.null = 0;
      return result;
    }
  }
  else if (strcmp(node->op, "*") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      ast_ *result = init_ast(AST_INTEGER);
      result->int_value.value = left_val->int_value.value * right_val->int_value.value;
      result->int_value.null = 0;
      return result;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      ast_ *result = init_ast(AST_REAL);
      result->real_value.value = left_val->real_value.value * right_val->real_value.value;
      result->real_value.null = 0;
      return result;
    }
  }
  else if (strcmp(node->op, "/") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      ast_ *result = init_ast(AST_INTEGER);
      result->int_value.value = left_val->int_value.value / right_val->int_value.value;
      result->int_value.null = 0;
      return result;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      ast_ *result = init_ast(AST_REAL);
      result->real_value.value = left_val->real_value.value / right_val->real_value.value;
      result->real_value.null = 0;
      return result;
    }
  }
  else if (strcmp(node->op, "^") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      ast_ *result = init_ast(AST_INTEGER);
      result->int_value.value = pow(left_val->int_value.value, right_val->int_value.value);
      result->int_value.null = 0;
      return result;
    }
    else if (left_val->type == AST_REAL && right_val->type == AST_REAL)
    {
      ast_ *result = init_ast(AST_REAL);
      result->real_value.value = pow(left_val->real_value.value, right_val->real_value.value);
      result->real_value.null = 0;
      return result;
    }
  }
  else if (strcmp(node->op, "DIV") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      ast_ *result = init_ast(AST_INTEGER);
      result->int_value.value = left_val->int_value.value / right_val->int_value.value; // Integer division
      result->int_value.null = 0;
      return result;
    }
  }
  else if (strcmp(node->op, "MOD") == 0)
  {
    if (left_val->type == AST_INTEGER && right_val->type == AST_INTEGER)
    {
      ast_ *result = init_ast(AST_INTEGER);
      result->int_value.value = left_val->int_value.value % right_val->int_value.value;
      result->int_value.null = 0;
      return result;
    }
  }

  printf("Invalid arithmetic operation: %s\n", node->op);
  exit(1);
  return init_ast(AST_NOOP); // Return a no-op if no valid operation found
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
  scope_add_instantiation_definition(
      get_scope(node),
      node);

  return node;
}
ast_ *interpreter_process_subroutine(interpreter_ *interpreter, ast_ *node)
{
  scope_add_instantiation_definition(
      get_scope(node),
      node);

  return node;
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
  // 1. Process the end expression (loop condition)
  ast_ *end = interpreter_process(interpreter, node->end_expr);
  if (end->type != AST_INTEGER || end->int_value.null == 1)
  {
    fprintf(stderr, "Error: End expression could not be recognized as a integer\n");
    return init_ast(AST_NOOP);
  }

  // 2. Process the step expression
  ast_ *step = interpreter_process(interpreter, node->step_expr);
  if (step->type != AST_INTEGER || step->int_value.null == 1)
  {
    fprintf(stderr, "Error: Step expression could not be recognized as a integer\n");
    return init_ast(AST_NOOP);
  }

  scope_ *local_scope = init_scope(node->scope);
  scope_add_variable_definition(local_scope, node->loop_variable);

  if (node->loop_variable->rhs->int_value.value <= end->int_value.value)
  {
    // 4. Loop through [loop_variable_value, end_value] using step_value
    while (node->loop_variable->rhs->int_value.value <= end->int_value.value)
    {
      for (int i = 0; node->loop_body[i] != NULL; i++)
      {
        ast_ *current_statement = node->loop_body[i];
        set_scope(current_statement, local_scope);

        interpreter_process(interpreter, current_statement);
      }

      // 6. Increment the loop variable by the step value
      node->loop_variable->rhs->int_value.value += step->int_value.value;
      scope_add_variable_definition(local_scope, node->loop_variable);
    }
  }
  else
  {
    // 4. Loop through [loop_variable_value, end_value] using step_value
    while (node->loop_variable->rhs->int_value.value >= end->int_value.value)
    {
      for (int i = 0; node->loop_body[i] != NULL; i++)
      {
        ast_ *current_statement = node->loop_body[i];
        set_scope(current_statement, local_scope);

        interpreter_process(interpreter, current_statement);
      }

      // 6. Increment the loop variable by the step value
      node->loop_variable->rhs->int_value.value += step->int_value.value;
      scope_add_variable_definition(local_scope, node->loop_variable);
    }
  }

  return init_ast(AST_NOOP); // Return a NOOP after loop execution
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
