#ifndef SCOPE_H
#define SCOPE_H
#include "ast.h"

typedef struct SCOPE_STRUCT
{
  ast_ **instantiation_definitions;
  ast_ **variable_definitions;

} scope_;

scope_ *init_scope();

void set_scope(ast_ *node, scope_ *scope);

scope_ *get_scope(ast_ *node);

ast_ *scope_add_instantiation_definition(scope_ *scope, ast_ *idef);

ast_ *scope_get_instantiation_definition(scope_ *scope, const char *iname);

ast_ *scope_add_variable_definition(scope_ *scope, ast_ *vdef);

ast_ *scope_get_variable_definition(scope_ *scope, const char *vname);
#endif