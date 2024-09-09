#include "include/scope.h"
#include <stdio.h>
#include <string.h>

scope_ *init_scope()
{
  scope_ *scope = calloc(1, sizeof(struct SCOPE_STRUCT));
  if (!scope)
  {
    fprintf(stderr, "Error: Memory allocation failed for scope initialization.\n");
    return NULL;
  }

  // Initialize empty lists for subroutines, variables, and records
  scope->subroutine_definitions = init_ast_list();
  scope->variable_definitions = init_ast_list();
  scope->record_definitions = init_ast_list();

  return scope;
}

ast_ *scope_add_subroutine_definition(scope_ *scope, ast_ *sdef)
{
  if (!scope || !sdef)
  {
    fprintf(stderr, "Error: Invalid scope or subroutine definition.\n");
    return NULL;
  }

  // Add the subroutine definition to the list
  add_ast_to_list(&scope->subroutine_definitions, sdef);

  return sdef;
}

ast_ *scope_get_subroutine_definition(scope_ *scope, const char *fname)
{
  if (!scope || !fname)
  {
    fprintf(stderr, "Error: Invalid scope or subroutine name.\n");
    return NULL;
  }

  for (size_t i = 0; scope->subroutine_definitions[i] != NULL; i++)
  {
    if (strcmp(scope->subroutine_definitions[i]->subroutine_name, fname) == 0)
    {
      return scope->subroutine_definitions[i];
    }
  }

  return NULL; // Subroutine not found
}

ast_ *scope_add_variable_definition(scope_ *scope, ast_ *vdef)
{
  if (!scope || !vdef)
  {
    fprintf(stderr, "Error: Invalid scope or variable definition.\n");
    return NULL;
  }

  // Variable name is accessed via vdef->lhs->variable_name
  const char *new_var_name = vdef->lhs->variable_name;

  // Iterate through the existing variable definitions to check for overwriting
  for (size_t i = 0; scope->variable_definitions[i] != NULL; i++)
  {
    if (strcmp(scope->variable_definitions[i]->lhs->variable_name, new_var_name) == 0)
    {
      // Overwrite the existing variable definition
      scope->variable_definitions[i]->lhs = vdef->lhs;
      scope->variable_definitions[i]->rhs = vdef->rhs;
      return scope->variable_definitions[i]; // Return the overwritten variable
    }
  }

  // If no existing variable was found, add the new definition to the list
  add_ast_to_list(&scope->variable_definitions, vdef);

  return vdef;
}

ast_ *scope_get_variable_definition(scope_ *scope, const char *vname)
{
  if (!scope || !vname)
  {
    fprintf(stderr, "Error: Invalid scope or variable name.\n");
    return NULL;
  }

  for (size_t i = 0; scope->variable_definitions[i] != NULL; i++)
  {
    if (strcmp(scope->variable_definitions[i]->lhs->variable_name, vname) == 0)
    {
      return scope->variable_definitions[i];
    }
  }

  return NULL; // Variable not found
}

ast_ *scope_add_record_definition(scope_ *scope, ast_ *rdef)
{
  if (!scope || !rdef)
  {
    fprintf(stderr, "Error: Invalid scope or record definition.\n");
    return NULL;
  }

  // Add the record definition to the list
  add_ast_to_list(&scope->record_definitions, rdef);

  return rdef;
}

ast_ *scope_get_record_definition(scope_ *scope, const char *rname)
{
  if (!scope || !rname)
  {
    fprintf(stderr, "Error: Invalid scope or record name.\n");
    return NULL;
  }

  for (size_t i = 0; scope->record_definitions[i] != NULL; i++)
  {
    if (strcmp(scope->record_definitions[i]->record_name, rname) == 0)
    {
      return scope->record_definitions[i];
    }
  }

  return NULL; // Record not found
}
