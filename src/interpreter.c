#include "include/interpreter.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

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
  printf(">> compound <<\n");
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
  printf(">> array <<\n");
  // Process each element in the array
  for (size_t i = 0; node->array_elements[i] != NULL; i++)
  {
    interpreter_process(interpreter, node->array_elements[i]);
  }

  return node; // Return the processed array node
}
ast_ *interpreter_process_assignment(interpreter_ *interpreter, ast_ *node)
{
  printf(">> assignment <<\n");

  // Process right-hand side before storing in scope
  ast_ *rhs_value = interpreter_process(interpreter, node->rhs);
  node->rhs = rhs_value; // Update the right-hand side with the processed value

  // Add the variable to the scope
  scope_add_variable_definition(node->scope, node);

  return node;
}

ast_ *interpreter_process_variable(interpreter_ *interpreter, ast_ *node)
{
  printf(">> variable <<\n");

  // Fetch variable definition from scope
  ast_ *vdef = scope_get_variable_definition(node->scope, node->variable_name);

  if (vdef != NULL)
  {
    // Process and return the left-hand side (i.e., the variable's value)
    return interpreter_process(interpreter, vdef->rhs);
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
  printf(">> array_access <<\n");

  // Fetch the array
  ast_ *array = interpreter_process_variable(interpreter, node->lhs);

  // Evaluate the index
  ast_ *index_ast = interpreter_process(interpreter, node->index);

  // Assuming the index is an integer
  int index = index_ast->int_value;

  if (array->array_elements[index] != NULL)
  {
    return array->array_elements[index];
  }

  printf("Index `%d` out of bounds for array.\n", index);
  exit(1);
  return init_ast(AST_NOOP);
}

ast_ *interpreter_process_instantiation(interpreter_ *interpreter, ast_ *node)
{
  printf(">> instantiation <<\n");
  return init_ast(AST_NOOP);
}
ast_ *interpreter_process_arithmetic_expression(interpreter_ *interpreter, ast_ *node)
{
  printf(">> arithmetic_expression <<\n");
  return init_ast(AST_NOOP);
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

  for (int i = node->start_expr->int_value; i <= node->end_expr->int_value; i++)
  {
    interpreter_process(interpreter, node->loop_body);
  }

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
