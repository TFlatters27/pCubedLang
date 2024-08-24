#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "ast.h"

typedef struct INTERPRETER_STRUCT
{
} interpreter_;

interpreter_ *init_interpreter();

ast_ *interpreter_process(interpreter_ *interpreter, ast_ *node);

ast_ *interpreter_process_compound(interpreter_ *interpreter, ast_ *node);
#endif