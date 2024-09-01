#include "include/ast.h"

const char *ast_type_to_string(enum ast_type type)
{
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
  case AST_NUM:
    return "AST_NUM";
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
  case AST_FUNCTION_CALL:
    return "AST_FUNCTION_CALL";
  case AST_ARRAY:
    return "AST_ARRAY";
  case AST_BINARY_OP:
    return "AST_BINARY_OP";
  case AST_UNARY_OP:
    return "AST_UNARY_OP";
  case AST_RECORD:
    return "AST_RECORD";
  default:
    return "UNKNOWN_AST_TYPE";
  }
}

ast_ *init_ast(enum ast_type type)
{
  ast_ *ast = calloc(1, sizeof(struct AST_STRUCT));
  ast->type = type;

  ast->scope = NULL;

  // TODO: init stuff to NULL or 0 e.g ast->variable_assignment_variable_name = NULL

  return ast;
}