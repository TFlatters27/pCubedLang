#include "include/ast.h"

const char *ast_type_to_string(enum ast_type type)
{
  return ("Unknown AST type");
}

ast_ *init_ast(enum ast_type type)
{
  ast_ *ast = calloc(1, sizeof(struct AST_STRUCT));
  ast->type = type;

  ast->scope = NULL;

  // TODO: init stuff to NULL or 0 e.g ast->variable_assignment_variable_name = NULL

  return ast;
}