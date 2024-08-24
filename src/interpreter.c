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
  return init_ast(AST_NOOP);
}

ast_ *interpreter_visit_compound(interpreter_ *interpreter, ast_ *node)
{
  return init_ast(AST_NOOP);
}
