#ifndef SCOPE_H
#define SCOPE_H
#include "ast.h"

typedef struct SCOPE_STRUCT
{
  ast_ **subroutine_definitions;
  ast_ **variable_definitions;
  ast_ **record_definitions;

} scope_;

scope_ *init_scope();

ast_ *scope_add_subroutine_definition(scope_ *scope, ast_ *fdef);

ast_ *scope_get_subroutine_definition(scope_ *scope, const char *fname);

ast_ *scope_add_variable_definition(scope_ *scope, ast_ *vdef);

ast_ *scope_get_variable_definition(scope_ *scope, const char *vname);

ast_ *scope_add_record_definition(scope_ *scope, ast_ *rdef);

ast_ *scope_get_record_definition(scope_ *scope, const char *rname);
#endif