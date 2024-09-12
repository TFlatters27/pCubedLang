#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "ast.h"

typedef struct INTERPRETER_STRUCT
{
  // Add any interpreter-specific fields here, like symbol tables, etc.
} interpreter_;

interpreter_ *init_interpreter();

ast_ *interpreter_process(interpreter_ *interpreter, ast_ *node);

// Methods for processing each AST type
ast_ *interpreter_process_compound(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_assignment(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_variable(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_record_access(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_array_access(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_instantiation(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_arithmetic_expression(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_boolean_expression(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_record_definition(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_subroutine(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_return(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_output(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_definite_loop(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_indefinite_loop(interpreter_ *interpreter, ast_ *node);
ast_ *interpreter_process_selection(interpreter_ *interpreter, ast_ *node);

#endif
