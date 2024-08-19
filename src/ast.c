#include "include/ast.h"

ast_ *init_ast(enum ast_type type)
{
  ast_ *ast = calloc(1, sizeof(struct AST_STRUCT));
  ast->type = type;

  ast->scope = NULL;

  /* AST_VARIABLE_DEFINITION */
  ast->variable_definition_variable_name = NULL;
  ast->variable_definition_value = NULL;

  /* AST_SUBROUTINE_DEFINITION */
  ast->subroutine_definition_body = NULL;
  ast->subroutine_definition_name = NULL;
  ast->subroutine_definition_args = NULL;
  ast->subroutine_definition_args_size = 0;

  /* AST_VARIABLE */
  ast->variable_name = NULL;

  /* AST_SUBROUTINE_CALL */
  ast->subroutine_call_name = NULL;
  ast->subroutine_call_arguments = NULL;
  ast->subroutine_call_arguments_size = 0;

  /* AST_STRING */
  ast->string_value = NULL;

  /* AST_COMPOUND */
  ast->compound_value = NULL;
  ast->compound_size = 0;

  return ast;
}