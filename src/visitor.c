#include "include/visitor.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

// Built-in subroutine for print
static ast_ *builtin_subroutine_output(visitor_ *visitor, ast_ **args, int args_size)
{
  for (int i = 0; i < args_size; i++)
  {
    ast_ *visited_ast = visitor_visit(visitor, args[i]);

    switch (visited_ast->type)
    {
    case AST_ARRAY:
      printf("%s\n", visited_ast->string_value);
      break;
    default:
      printf("%p\n", visited_ast);
      break;
    }
  }

  return init_ast(AST_NOOP);
}

// Built-in subroutine for exit
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

// Built-in subroutine for clearing the console
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

// Initialize the visitor
visitor_ *init_visitor()
{
  visitor_ *visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
  return visitor;
}

// Define the main visit function
ast_ *visitor_visit(visitor_ *visitor, ast_ *node)
{
  switch (node->type)
  {
  case AST_VARIABLE_ASSIGNMENT:
    return visitor_visit_variable_definition(visitor, node);
  case AST_SUBROUTINE_DEFINITION:
    return visitor_visit_subroutine_definition(visitor, node);
  case AST_VARIABLE:
    return visitor_visit_variable(visitor, node);
  case AST_SUBROUTINE_CALL:
    return visitor_visit_subroutine_call(visitor, node);
  case AST_ARRAY:
    return visitor_visit_string(visitor, node);
  case AST_COMPOUND:
    return visitor_visit_compound(visitor, node);
  case AST_NOOP:
    return node;
  }

  printf("Uncaught statement of type `%d`\n", node->type);
  exit(1);

  return init_ast(AST_NOOP);
}

// Handle variable definitions
ast_ *visitor_visit_variable_definition(visitor_ *visitor, ast_ *node)
{
  scope_add_variable_definition(node->scope, node);
  return node;
}

// Handle subroutine definitions
ast_ *visitor_visit_subroutine_definition(visitor_ *visitor, ast_ *node)
{
  scope_add_subroutine_definition(node->scope, node);
  return node;
}

// Handle variables
ast_ *visitor_visit_variable(visitor_ *visitor, ast_ *node)
{
  ast_ *vdef = scope_get_variable_definition(node->scope, node->variable_name);

  if (vdef != (void *)0)
  {
    return visitor_visit(visitor, vdef->variable_definition_value);
  }

  printf("Undefined variable `%s`\n", node->variable_name);
  exit(1);
}

// Handle subroutine calls
ast_ *visitor_visit_subroutine_call(visitor_ *visitor, ast_ *node)
{
  if (strcmp(node->subroutine_call_name, "OUTPUT") == 0)
  {
    return builtin_subroutine_output(visitor, node->subroutine_call_arguments, node->subroutine_call_arguments_size);
  }
  if (strcmp(node->subroutine_call_name, "EXIT") == 0)
  {
    return builtin_subroutine_exit(visitor, node->subroutine_call_arguments, node->subroutine_call_arguments_size);
  }
  if (strcmp(node->subroutine_call_name, "CLEAR") == 0)
  {
    return builtin_subroutine_clear(visitor, node->subroutine_call_arguments, node->subroutine_call_arguments_size);
  }

  ast_ *fdef = scope_get_subroutine_definition(node->scope, node->subroutine_call_name);

  if (fdef == (void *)0)
  {
    printf("Undefined method `%s`\n", node->subroutine_call_name);
    exit(1);
  }

  for (int i = 0; i < (int)node->subroutine_call_arguments_size; i++)
  {
    // Grab the variable from the subroutine definition arguments
    ast_ *ast_var = (ast_ *)fdef->subroutine_definition_args[i];

    // Grab the value from the subroutine call arguments
    ast_ *ast_value = (ast_ *)node->subroutine_call_arguments[i];

    // Create a new variable definition with the value of the argument in the subroutine call
    ast_ *ast_vardef = init_ast(AST_VARIABLE_ASSIGNMENT);
    ast_vardef->variable_definition_value = ast_value;

    // Copy the name from the subroutine definition argument into the new variable definition
    ast_vardef->variable_definition_variable_name = (char *)calloc(strlen(ast_var->variable_name) + 1, sizeof(char));
    strcpy(ast_vardef->variable_definition_variable_name, ast_var->variable_name);

    // Push the variable definition into the subroutine body scope
    scope_add_variable_definition(fdef->subroutine_definition_body->scope, ast_vardef);
  }

  return visitor_visit(visitor, fdef->subroutine_definition_body);
}

// Handle strings
ast_ *visitor_visit_string(visitor_ *visitor, ast_ *node)
{
  return node;
}

// Handle compound statements
ast_ *visitor_visit_compound(visitor_ *visitor, ast_ *node)
{
  for (int i = 0; i < node->compound_size; i++)
  {
    visitor_visit(visitor, node->compound_value[i]);
  }

  return init_ast(AST_NOOP);
}
