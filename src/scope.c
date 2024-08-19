#include "include/scope.h"
#include <string.h>

scope_ *init_scope()
{
  scope_ *scope = calloc(1, sizeof(struct SCOPE_STRUCT));

  scope->subroutine_definitions = (void *)0;
  scope->subroutine_definitions_size = 0;

  scope->variable_definitions = (void *)0;
  scope->variable_definitions_size = 0;

  return scope;
}

ast_ *scope_add_subroutine_definition(scope_ *scope, ast_ *fdef)
{
  scope->subroutine_definitions_size += 1;

  if (scope->subroutine_definitions == (void *)0)
  {
    scope->subroutine_definitions = calloc(1, sizeof(struct AST_STRUCT *));
  }
  else
  {
    scope->subroutine_definitions =
        realloc(
            scope->subroutine_definitions,
            scope->subroutine_definitions_size * sizeof(struct AST_STRUCT **));
  }

  scope->subroutine_definitions[scope->subroutine_definitions_size - 1] =
      fdef;

  return fdef;
}

ast_ *scope_get_subroutine_definition(scope_ *scope, const char *fname)
{
  for (int i = 0; i < scope->subroutine_definitions_size; i++)
  {
    ast_ *fdef = scope->subroutine_definitions[i];

    if (strcmp(fdef->subroutine_definition_name, fname) == 0)
    {
      return fdef;
    }
  }

  return (void *)0;
}

ast_ *scope_add_variable_definition(scope_ *scope, ast_ *vdef)
{
  if (scope->variable_definitions == (void *)0)
  {
    scope->variable_definitions = calloc(1, sizeof(struct AST_STRUCT *));
    scope->variable_definitions[0] = vdef;
    scope->variable_definitions_size += 1;
  }
  else
  {
    scope->variable_definitions_size += 1;
    scope->variable_definitions = realloc(
        scope->variable_definitions,
        scope->variable_definitions_size * sizeof(struct AST_STRUCT *));
    scope->variable_definitions[scope->variable_definitions_size - 1] = vdef;
  }

  return vdef;
}

ast_ *scope_get_variable_definition(scope_ *scope, const char *name)
{
  for (int i = 0; i < scope->variable_definitions_size; i++)
  {
    ast_ *vdef = scope->variable_definitions[i];

    if (strcmp(vdef->variable_definition_variable_name, name) == 0)
    {
      return vdef;
    }
  }

  return (void *)0;
}