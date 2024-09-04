#include "include/ast.h"
#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

const char *ast_type_to_string(enum ast_type type)
{
  static char buffer[50]; // static so it persists after function returns
  switch (type)
  {
  case AST_ASSIGNMENT:
    return "AST_ASSIGNMENT";
  case AST_INSTANTIATION:
    return "AST_INSTANTIATION";
  case AST_COMPOUND:
    return "AST_COMPOUND";
  case AST_NOOP:
    return "AST_NOOP";
  case AST_INT:
    return "AST_INT";
  case AST_STRING:
    return "AST_STRING";
  case AST_BOOL:
    return "AST_BOOL";
  case AST_CHAR:
    return "AST_CHAR";
  case AST_REAL:
    return "AST_REAL";
  case AST_VARIABLE:
    return "AST_VARIABLE";
  case AST_ARRAY:
    return "AST_ARRAY";
  case AST_BINARY_OP:
    return "AST_BINARY_OP";
  case AST_RELATIONAL_OP:
    return "AST_RELATIONAL_OP";
  case AST_UNARY_OP:
    return "AST_UNARY_OP";
  case AST_RECORD:
    return "AST_RECORD";
  case AST_RECORD_ACCESS:
    return "AST_RECORD_ACCESS";
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
    snprintf(buffer, sizeof(buffer), "UNKNOWN_AST_TYPE %d", type);
    return buffer;
  }
}

ast_ *init_ast(enum ast_type type)
{
  printf("\tCREATED AST: %s\n", ast_type_to_string(type));
  ast_ *ast = calloc(1, sizeof(struct AST_STRUCT));
  ast->type = type;

  ast->scope = NULL;

  // TODO: init stuff to NULL or 0 e.g ast->variable_assignment_variable_name = NULL

  return ast;
}