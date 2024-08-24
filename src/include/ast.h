#ifndef AST_H
#define AST_H
#include <stdlib.h>

enum ast_type
{
  AST_VARIABLE_ASSIGNMENT,
  AST_VARIABLE,
  AST_INTEGER_EXPRESSION,
  AST_BOOLEAN_EXPRESSION,
  AST_INDEFINITE_ITERATION,
  AST_DEFINITE_ITERATION,
  AST_SELECTION,
  AST_RECORD,
  AST_SUBROUTINE_DEFINITION,
  AST_SUBROUTINE_CALL,
  AST_ARRAY,
  AST_COMPOUND,
  AST_NOOP
};

typedef struct AST_STRUCT
{
  enum ast_type type;
  struct SCOPE_STRUCT *scope;
  char* string_value;

  /* AST_VARIABLE_ASSIGNMENT */
  char *variable_assignment_variable_name;
  struct AST_STRUCT *variable_assignment_value;

  /* AST_VARIABLE */
  char *variable_name;

  /* AST_INTEGER_EXPRESSION */
  int integer_value;

  /* AST_BOOLEAN_EXPRESSION */
  int boolean_value;

  /* AST_INDEFINITE_ITERATION */
  struct AST_STRUCT *indefinite_iteration_condition;
  struct AST_STRUCT *indefinite_iteration_body;

/* AST_DEFINITE_ITERATION */
  struct AST_STRUCT *definite_iteration_variable;
  int definite_iteration_start;
  int definite_iteration_end;
  int definite_iteration_step;
  struct AST_STRUCT *definite_iteration_collection;
  struct AST_STRUCT *definite_iteration_body;

  /* AST_SELECTION */
  struct AST_STRUCT *selection_condition;
  struct AST_STRUCT *selection_true_case;
  struct AST_STRUCT *selection_false_case;

  /* AST_RECORD */
  char *record_name;
  struct AST_STRUCT **record_fields;
  size_t record_fields_size;

  /* AST_SUBROUTINE_DEFINITION */
  struct AST_STRUCT *subroutine_definition_body;
  char *subroutine_definition_name;
  struct AST_STRUCT **subroutine_definition_args;
  size_t subroutine_definition_args_size;

  /* AST_SUBROUTINE_CALL */
  char *subroutine_call_name;
  struct AST_STRUCT **subroutine_call_arguments;
  size_t subroutine_call_arguments_size;

  /* AST_ARRAY */
  struct AST_STRUCT **array_elements;
  size_t array_size;

  /* AST_COMPOUND */
  struct AST_STRUCT **compound_value;
  size_t compound_size;

  /* AST_NOOP */
  // No operation, no additional fields needed.

} ast_;

const char *ast_type_to_string(enum ast_type type);
ast_ *init_ast(enum ast_type type);
#endif
