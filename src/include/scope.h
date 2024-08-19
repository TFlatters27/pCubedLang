#ifndef SCOPE_H
#define SCOPE_H
#include "ast.h"

typedef struct SCOPE_STRUCT
{
  ast_ **subroutine_definitions;
  size_t subroutine_definitions_size;

  ast_ **variable_definitions;
  size_t variable_definitions_size;
} scope_;

scope_ *init_scope();

ast_ *scope_add_subroutine_definition(scope_ *scope, ast_ *fdef);

ast_ *scope_get_subroutine_definition(scope_ *scope, const char *fname);

ast_ *scope_add_variable_definition(scope_ *scope, ast_ *vdef);

ast_ *scope_get_variable_definition(scope_ *scope, const char *name);
#endif