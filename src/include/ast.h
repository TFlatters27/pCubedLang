#ifndef AST_H
#define AST_H
#include <stdlib.h>

enum ast_type
{
  AST_VARIABLE_DEFINITION,
  AST_SUBROUTINE_DEFINITION,
  AST_VARIABLE,
  AST_SUBROUTINE_CALL,
  AST_STRING,
  AST_COMPOUND,
  AST_NOOP
};

typedef struct AST_STRUCT
{
  enum ast_type type;
  struct SCOPE_STRUCT *scope;

  /* AST_VARIABLE_DEFINITION */
  char *variable_definition_variable_name;
  struct AST_STRUCT *variable_definition_value;

  /* AST_VARIABLE */
  char *variable_name;

  /* AST_FUNCTION_DEFINITION */
  struct AST_STRUCT *subroutine_definition_body;
  char *subroutine_definition_name;
  struct AST_STRUCT **subroutine_definition_args;
  size_t subroutine_definition_args_size;

  /* AST_FUNCTION_CALL */
  char *subroutine_call_name;
  struct AST_STRUCT **subroutine_call_arguments;
  size_t subroutine_call_arguments_size;

  /* AST_STRING */
  char *string_value;

  /* AST_COMPOUND */
  struct AST_STRUCT **compound_value;
  size_t compound_size;
} ast_;

ast_ *init_ast(enum ast_type type);
#endif