#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"

typedef struct VISITOR_STRUCT
{
} visitor_;

visitor_ *init_visitor();

ast_ *visitor_visit(visitor_ *visitor, ast_ *node);

ast_ *visitor_visit_variable_definition(visitor_ *visitor, ast_ *node);

ast_ *visitor_visit_subroutine_definition(visitor_ *visitor, ast_ *node);

ast_ *visitor_visit_variable(visitor_ *visitor, ast_ *node);

ast_ *visitor_visit_subroutine_call(visitor_ *visitor, ast_ *node);

ast_ *visitor_visit_string(visitor_ *visitor, ast_ *node);

ast_ *visitor_visit_compound(visitor_ *visitor, ast_ *node);
#endif