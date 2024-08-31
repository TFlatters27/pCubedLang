#include "include/scope.h"
#include <string.h>

scope_ *init_scope()
{
  scope_ *scope = calloc(1, sizeof(struct SCOPE_STRUCT));

  scope->subroutine_definitions = NULL;
  scope->subroutine_definitions_size = 0;

  scope->variable_definitions = NULL;
  scope->variable_definitions_size = 0;

  return scope;
}

ast_ *scope_add_subroutine_definition(scope_ *scope, ast_ *fdef)
{

  return NULL;
}

ast_ *scope_get_subroutine_definition(scope_ *scope, const char *fname)
{

  return NULL;
}

ast_ *scope_add_variable_definition(scope_ *scope, ast_ *vdef)
{

  return NULL;
}

ast_ *scope_get_variable_definition(scope_ *scope, const char *name)
{

  return NULL;
}