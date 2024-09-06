#include "include/ast.h"
#include <stdio.h>
#include <string.h>

// Function to initialize an AST node of a given type.
ast_ *init_ast(enum ast_type type)
{
  ast_ *ast = calloc(1, sizeof(struct AST_STRUCT));
  ast->type = type;

  // Initialize common fields
  ast->scope = NULL;

  if (type == AST_NOOP)
  {
    printf("\n");
  }
  else
  {
    printf("Initializing AST node %s\n", ast_type_to_string(ast->type));
  }

  // Initialize based on the specific AST type
  switch (type)
  {
  case AST_COMPOUND:
    ast->compound_value = NULL;
    ast->compound_size = 0;
    break;

  case AST_INTEGER:
    ast->int_value = 0;
    break;

  case AST_REAL:
    ast->real_value = 0.0;
    break;

  case AST_CHARACTER:
    ast->char_value = '\0';
    break;

  case AST_STRING:
    ast->string_value = NULL;
    break;

  case AST_BOOLEAN:
    ast->boolean_value = 0;
    break;

  case AST_ARRAY:
    ast->array_elements = NULL;
    break;

  case AST_ASSIGNMENT:
    ast->lhs = NULL;
    ast->rhs = NULL;
    ast->constant = 0;
    break;

  case AST_VARIABLE:
    ast->variable_name = NULL;
    break;

  case AST_RECORD_ACCESS:
    ast->field_name = NULL;
    break;

  case AST_ARRAY_ACCESS:
    ast->index = NULL;
    break;

  case AST_INSTANTIATION:
    ast->class_name = NULL;
    ast->arguments = NULL;
    break;

  case AST_ARITHMETIC_EXPRESSION:
  case AST_BOOLEAN_EXPRESSION:
    ast->left = NULL;
    ast->right = NULL;
    ast->op = NULL;
    break;

  case AST_RECORD:
    ast->record_name = NULL;
    ast->record_elements = NULL;
    break;

  case AST_SUBROUTINE:
    ast->subroutine_name = NULL;
    ast->parameters = NULL;
    ast->body = NULL;
    break;

  case AST_RETURN:
    ast->return_value = NULL;
    break;

  case AST_OUTPUT:
    ast->output_expressions_size = 0;
    ast->output_expressions = NULL;
    break;

  case AST_DEFINITE_LOOP:
    ast->loop_variable = NULL;
    ast->start_expr = NULL;
    ast->end_expr = NULL;
    ast->step_expr = NULL;
    ast->collection_expr = NULL;
    break;

  case AST_INDEFINITE_LOOP:
    ast->condition = NULL;
    ast->loop_body = NULL;
    break;

  case AST_SELECTION:
    ast->if_condition = NULL;
    ast->if_body = NULL;
    ast->else_if_conditions = NULL;
    ast->else_if_bodies = NULL;
    ast->else_body = NULL;
    break;

  case AST_NOOP:
  default:
    // No-op doesn't need initialization beyond the type
    break;
  }

  return ast;
}

// Function to convert an AST type to a string representation.
const char *ast_type_to_string(enum ast_type type)
{
  switch (type)
  {
  case AST_COMPOUND:
    return "AST_COMPOUND";
  case AST_NOOP:
    return "AST_NOOP";
  case AST_INTEGER:
    return "AST_INTEGER";
  case AST_REAL:
    return "AST_REAL";
  case AST_CHARACTER:
    return "AST_CHARACTER";
  case AST_STRING:
    return "AST_STRING";
  case AST_BOOLEAN:
    return "AST_BOOLEAN";
  case AST_ARRAY:
    return "AST_ARRAY";
  case AST_ASSIGNMENT:
    return "AST_ASSIGNMENT";
  case AST_VARIABLE:
    return "AST_VARIABLE";
  case AST_RECORD_ACCESS:
    return "AST_RECORD_ACCESS";
  case AST_ARRAY_ACCESS:
    return "AST_ARRAY_ACCESS";
  case AST_INSTANTIATION:
    return "AST_INSTANTIATION";
  case AST_ARITHMETIC_EXPRESSION:
    return "AST_ARITHMETIC_EXPRESSION";
  case AST_BOOLEAN_EXPRESSION:
    return "AST_BOOLEAN_EXPRESSION";
  case AST_RECORD:
    return "AST_RECORD";
  case AST_SUBROUTINE:
    return "AST_SUBROUTINE";
  case AST_RETURN:
    return "AST_RETURN";
  case AST_OUTPUT:
    return "AST_OUTPUT";
  case AST_DEFINITE_LOOP:
    return "AST_DEFINITE_LOOP";
  case AST_INDEFINITE_LOOP:
    return "AST_INDEFINITE_LOOP";
  case AST_SELECTION:
    return "AST_SELECTION";
  default:
    return "AST_UNKNOWN";
  }
}
