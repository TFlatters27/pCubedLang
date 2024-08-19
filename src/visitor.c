#include "include/visitor.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

static ast_ *builtin_subroutine_print(visitor_ *visitor, ast_ **args, int args_size)
{
  for (int i = 0; i < args_size; i++)
  {
    ast_ *visited_ast = visitor_visit(visitor, args[i]);

    switch (visited_ast->type)
    {
    case AST_STRING:
      printf("%s\n", visited_ast->string_value);
      break;
    default:
      printf("%p\n", visited_ast);
      break;
    }
  }

  return init_ast(AST_NOOP);
}

static ast_ *builtin_subroutine_exit(visitor_ *visitor, ast_ **args, int args_size)
{
  for (int i = 0; i < args_size; i++)
  {
    ast_ *visited_ast = visitor_visit(visitor, args[i]);

    switch (visited_ast->type)
    {
    case AST_NOOP:
      printf("You exited\n");
      exit(0);
      break;
    default:
      printf("%p\n", visited_ast);
      break;
    }
  }

  return init_ast(AST_NOOP);
}

static ast_ *builtin_subroutine_clear(visitor_ *visitor, ast_ **args, int args_size)
{
  for (int i = 0; i < args_size; i++)
  {
    ast_ *visited_ast = visitor_visit(visitor, args[i]);

    switch (visited_ast->type)
    {
    case AST_NOOP:
      system("clear");
      break;
    default:
      printf("%p\n", visited_ast);
      break;
    }
  }

  return init_ast(AST_NOOP);
}

visitor_ *init_visitor()
{
  visitor_ *visitor = calloc(1, sizeof(struct VISITOR_STRUCT));

  return visitor;
}

ast_ *visitor_visit(visitor_ *visitor, ast_ *node)
{
  switch (node->type)
  {
  case AST_VARIABLE_DEFINITION:
    return visitor_visit_variable_definition(visitor, node);
    break;
  case AST_SUBROUTINE_DEFINITION:
    return visitor_visit_subroutine_definition(visitor, node);
    break;
  case AST_VARIABLE:
    return visitor_visit_variable(visitor, node);
    break;
  case AST_SUBROUTINE_CALL:
    return visitor_visit_subroutine_call(visitor, node);
    break;
  case AST_STRING:
    return visitor_visit_string(visitor, node);
    break;
  case AST_COMPOUND:
    return visitor_visit_compound(visitor, node);
    break;
  case AST_NOOP:
    return node;
    break;
  }

  printf("Uncaught statement of type `%d`\n", node->type);
  exit(1);

  return init_ast(AST_NOOP);
}

ast_ *visitor_visit_variable_definition(visitor_ *visitor, ast_ *node)
{
  scope_add_variable_definition(
      node->scope,
      node);

  return node;
}

ast_ *visitor_visit_subroutine_definition(visitor_ *visitor, ast_ *node)
{
  scope_add_subroutine_definition(
      node->scope,
      node);

  return node;
}

ast_ *visitor_visit_variable(visitor_ *visitor, ast_ *node)
{
  ast_ *vdef = scope_get_variable_definition(
      node->scope,
      node->variable_name);

  if (vdef != (void *)0)
    return visitor_visit(visitor, vdef->variable_definition_value);

  printf("Undefined variable `%s`\n", node->variable_name);
  exit(1);
}

ast_ *visitor_visit_subroutine_call(visitor_ *visitor, ast_ *node)
{
  if (strcmp(node->subroutine_call_name, "print") == 0)
  {
    return builtin_subroutine_print(visitor, node->subroutine_call_arguments, node->subroutine_call_arguments_size);
  }

  if (strcmp(node->subroutine_call_name, "exit") == 0)
  {
    return builtin_subroutine_exit(visitor, node->subroutine_call_arguments, node->subroutine_call_arguments_size);
  }

  if (strcmp(node->subroutine_call_name, "clear") == 0)
  {
    return builtin_subroutine_clear(visitor, node->subroutine_call_arguments, node->subroutine_call_arguments_size);
  }

  ast_ *fdef = scope_get_subroutine_definition(
      node->scope,
      node->subroutine_call_name);

  if (fdef == (void *)0)
  {
    printf("Undefined method `%s`\n", node->subroutine_call_name);
    exit(1);
  }

  for (int i = 0; i < (int)node->subroutine_call_arguments_size; i++)
  {
    // grab the variable from the subroutine definition arguments
    ast_ *ast_var = (ast_ *)fdef->subroutine_definition_args[i];

    // grab the value from the subroutine call arguments
    ast_ *ast_value = (ast_ *)node->subroutine_call_arguments[i];

    // create a new variable definition with the value of the argument
    // in the subroutine call.
    ast_ *ast_vardef = init_ast(AST_VARIABLE_DEFINITION);
    ast_vardef->variable_definition_value = ast_value;

    // copy the name from the subroutine definition argument into the new
    // variable definition
    ast_vardef->variable_definition_variable_name = (char *)calloc(strlen(ast_var->variable_name) + 1, sizeof(char));
    strcpy(ast_vardef->variable_definition_variable_name, ast_var->variable_name);

    // push our variable definition into the subroutine body scope.
    scope_add_variable_definition(fdef->subroutine_definition_body->scope, ast_vardef);
  }

  return visitor_visit(visitor, fdef->subroutine_definition_body);
}

ast_ *visitor_visit_string(visitor_ *visitor, ast_ *node)
{
  return node;
}

ast_ *visitor_visit_compound(visitor_ *visitor, ast_ *node)
{
  for (int i = 0; i < node->compound_size; i++)
  {
    visitor_visit(visitor, node->compound_value[i]);
  }

  return init_ast(AST_NOOP);
}